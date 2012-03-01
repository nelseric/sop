#include "SoPGoalKeeperStates.h"
#include "Debug/DebugConsole.h"
#include "../../SoccerPitch.h"
#include "../../PlayerBase.h"
#include "../../GoalKeeper.h"
#include "../../SteeringBehaviors.h"
#include "SonsOfPitches.h"
#include "../../Goal.h"
#include "2D/geometry.h"
#include "../../FieldPlayer.h"
#include "../../ParamLoader.h"
#include "Messaging/Telegram.h"
#include "Messaging/MessageDispatcher.h"
#include "../../SoccerMessages.h"


//uncomment to send state info to debug window
//#define GOALY_STATE_INFO_ON


//--------------------------- SoPGlobalKeeperState -------------------------------
//-----------------------------------------------------------------------------

SoPGlobalKeeperState* SoPGlobalKeeperState::Instance()
{
  static SoPGlobalKeeperState instance;

  return &instance;
}


bool SoPGlobalKeeperState::OnMessage(GoalKeeper* keeper, const Telegram& telegram)
{
  switch(telegram.Msg)
  {
    case Msg_GoHome:
    {
      keeper->SetDefaultHomeRegion();
      
      keeper->GetFSM()->ChangeState(SoPReturnHome::Instance());
    }

    break;

    case Msg_ReceiveBall:
      {
        keeper->GetFSM()->ChangeState(SoPInterceptBall::Instance());
      }

      break;

  }//end switch

  return false;
}


//--------------------------- SoPTendGoal -----------------------------------
//
//  This is the main state for the goalkeeper. When in this state he will
//  move left to right across the goalmouth using the 'interpose' steering
//  behavior to put himself between the ball and the back of the net.
//
//  If the ball comes within the 'goalkeeper range' he moves out of the
//  goalmouth to attempt to intercept it. (see next state)
//------------------------------------------------------------------------

SoPTendGoal* SoPTendGoal::Instance()
{
  static SoPTendGoal instance;

  return &instance;
}


void SoPTendGoal::Enter(GoalKeeper* keeper)
{
  //turn interpose on
  keeper->Steering()->InterposeOn(Prm.GoalKeeperTendingDistance);

  //interpose will position the agent between the ball position and a target
  //position situated along the goal mouth. This call sets the target
  keeper->Steering()->SetTarget(keeper->GetRearInterposeTarget());
}

void SoPTendGoal::Execute(GoalKeeper* keeper)
{
  //the rear interpose target will change as the ball's position changes
  //so it must be updated each update-step 
  keeper->Steering()->SetTarget(keeper->GetRearInterposeTarget());

  //if the ball comes in range the keeper traps it and then changes state
  //to put the ball back in play
  if (keeper->BallWithinKeeperRange())
  {
    keeper->Ball()->Trap();

    keeper->Pitch()->SetGoalKeeperHasBall(true);

    keeper->GetFSM()->ChangeState(SoPPutBallBackInPlay::Instance());

    return;
  }

  //if ball is within a predefined distance, the keeper moves out from
  //position to try and intercept it.
  if (keeper->BallWithinRangeForIntercept() && !keeper->Team()->InControl())
  {
    keeper->GetFSM()->ChangeState(SoPInterceptBall::Instance());
  }

  //if the keeper has ventured too far away from the goal-line and there
  //is no threat from the opponents he should move back towards it
  if (keeper->TooFarFromGoalMouth() && keeper->Team()->InControl())
  {
    keeper->GetFSM()->ChangeState(SoPReturnHome::Instance());

    return;
  }
}


void SoPTendGoal::Exit(GoalKeeper* keeper)
{
  keeper->Steering()->InterposeOff();
}


//------------------------- SoPReturnHome: ----------------------------------
//
//  In this state the goalkeeper simply returns back to the center of
//  the goal region before changing state back to SoPTendGoal
//------------------------------------------------------------------------

SoPReturnHome* SoPReturnHome::Instance()
{
  static SoPReturnHome instance;

  return &instance;
}


void SoPReturnHome::Enter(GoalKeeper* keeper)
{
  keeper->Steering()->ArriveOn();
}

void SoPReturnHome::Execute(GoalKeeper* keeper)
{
  keeper->Steering()->SetTarget(keeper->HomeRegion()->Center());

  //if close enough to home or the opponents get control over the ball,
  //change state to tend goal
  if (keeper->InHomeRegion() || !keeper->Team()->InControl())
  {
    keeper->GetFSM()->ChangeState(SoPTendGoal::Instance());
  }
}

void SoPReturnHome::Exit(GoalKeeper* keeper)
{
  keeper->Steering()->ArriveOff();
}



//----------------- SoPInterceptBall ----------------------------------------
//
//  In this state the GP will attempt to intercept the ball using the
//  pursuit steering behavior, but he only does so so long as he remains
//  within his home region.
//------------------------------------------------------------------------

SoPInterceptBall* SoPInterceptBall::Instance()
{
  static SoPInterceptBall instance;

  return &instance;
}


void SoPInterceptBall::Enter(GoalKeeper* keeper)
{
  keeper->Steering()->PursuitOn();  

    #ifdef GOALY_STATE_INFO_ON
    debug_con << "Goaly " << keeper->ID() << " enters SoPInterceptBall" <<  "";
    #endif
}

void SoPInterceptBall::Execute(GoalKeeper* keeper)
{ 
  //if the goalkeeper moves to far away from the goal he should return to his
  //home region UNLESS he is the closest player to the ball, in which case,
  //he should keep trying to intercept it.
  if (keeper->TooFarFromGoalMouth() && !keeper->isClosestPlayerOnPitchToBall())
  {
    keeper->GetFSM()->ChangeState(SoPReturnHome::Instance());

    return;
  }
  
  //if the ball becomes in range of the goalkeeper's hands he traps the 
  //ball and puts it back in play
  if (keeper->BallWithinKeeperRange())
  {
    keeper->Ball()->Trap();
    
    keeper->Pitch()->SetGoalKeeperHasBall(true);

    keeper->GetFSM()->ChangeState(SoPPutBallBackInPlay::Instance());

    return;
  }
}

void SoPInterceptBall::Exit(GoalKeeper* keeper)
{
  keeper->Steering()->PursuitOff();
}



//--------------------------- SoPPutBallBackInPlay --------------------------
//
//------------------------------------------------------------------------

SoPPutBallBackInPlay* SoPPutBallBackInPlay::Instance()
{
  static SoPPutBallBackInPlay instance;

  return &instance;
}

void SoPPutBallBackInPlay::Enter(GoalKeeper* keeper)
{
  //let the team know that the keeper is in control
  keeper->Team()->SetControllingPlayer(keeper);

  //send all the players home
  keeper->Team()->Opponents()->ReturnAllFieldPlayersToHome();
  keeper->Team()->ReturnAllFieldPlayersToHome();
}


void SoPPutBallBackInPlay::Execute(GoalKeeper* keeper)
{
  PlayerBase*  receiver = NULL;
  Vector2D     BallTarget;
    
  //test if there are players further forward on the field we might
  //be able to pass to. If so, make a pass.
  if (keeper->Team()->FindPass(keeper,
                              receiver,
                              BallTarget,
                              Prm.MaxPassingForce,
                              Prm.GoalkeeperMinPassDist))
  {     
    //make the pass   
    keeper->Ball()->Kick(Vec2DNormalize(BallTarget - keeper->Ball()->Pos()),
                         Prm.MaxPassingForce);

    //goalkeeper no longer has ball 
    keeper->Pitch()->SetGoalKeeperHasBall(false);

    //let the receiving player know the ball's comin' at him
    Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
                          keeper->ID(),
                          receiver->ID(),
                          Msg_ReceiveBall,
                          &BallTarget);
    
    //go back to tending the goal   
    keeper->GetFSM()->ChangeState(SoPTendGoal::Instance());

    return;
  }  

  keeper->SetVelocity(Vector2D());
}

