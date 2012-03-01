#include "SonsOfPitches.h"
#include "../../SoccerPitch.h"
#include "../../Goal.h"
#include "../../PlayerBase.h"
#include "../../GoalKeeper.h"
#include "../../FieldPlayer.h"
#include "misc/utils.h"
#include "../../SteeringBehaviors.h"
#include "SoPFieldPlayerStates.h"
#include "SoPGoalKeeperStates.h"
#include "../../ParamLoader.h"
#include "2D/geometry.h"
#include "Game/EntityManager.h"
#include "Messaging/MessageDispatcher.h"
#include "../../SoccerMessages.h"
#include "SoPTeamStates.h"
#include "Debug/DebugConsole.h"
#include <windows.h>
using std::vector;


SonsOfPitches::SonsOfPitches(Goal*        home_goal,
                       Goal*        opponents_goal,
                       SoccerPitch* pitch,
					   team_color   color): AbstSoccerTeam(home_goal, opponents_goal, pitch, color)
{
	InitStateMachine();
	CreatePlayers();
	RegisterPlayers();
	InitPlayers();

  //create the sweet spot calculator
  m_pSupportSpotCalc = new SupportSpotCalculator(Prm.NumSupportSpotsX,
                                                 Prm.NumSupportSpotsY,
                                                 this);
}


//----------------------- dtor -------------------------------------------
//
//------------------------------------------------------------------------
SonsOfPitches::~SonsOfPitches()
{

  delete m_pSupportSpotCalc;
}

void SonsOfPitches::InitStateMachine()
{
  m_pStateMachine = new StateMachine<AbstSoccerTeam>(this);

  m_pStateMachine->SetCurrentState(SoPDefending::Instance());
  m_pStateMachine->SetPreviousState(SoPDefending::Instance());
  m_pStateMachine->SetGlobalState(NULL);
}

void SonsOfPitches::InitPlayers()
{
  //set default steering behaviors
  std::vector<PlayerBase*>::iterator it = m_Players.begin();

  for (it; it != m_Players.end(); ++it)
  {
    (*it)->Steering()->SeparationOn();   
  }
}



//------------------------- CreatePlayers --------------------------------
//
//  creates the players
//------------------------------------------------------------------------
void SonsOfPitches::CreatePlayers()
{
  if (Color() == blue)
  {
    //goalkeeper
    m_Players.push_back(new GoalKeeper(this,
                               1,
                               SoPTendGoal::Instance(),
							   SoPGlobalKeeperState::Instance(),
                               Vector2D(0,1),
                               Vector2D(0.0, 0.0),
                               Prm.PlayerMass,
                               Prm.PlayerMaxForce,
                               Prm.PlayerMaxSpeedWithoutBall,
                               Prm.PlayerMaxTurnRate,
                               Prm.PlayerScale));
 
    //create the players
    m_Players.push_back(new FieldPlayer(this,
                               6,
                               SoPWait::Instance(),
							   SoPGlobalPlayerState::Instance(),
                               Vector2D(0,1),
                               Vector2D(0.0, 0.0),
                               Prm.PlayerMass,
                               Prm.PlayerMaxForce,
                               Prm.PlayerMaxSpeedWithoutBall,
                               Prm.PlayerMaxTurnRate,
                               Prm.PlayerScale,
                               PlayerBase::attacker));



        m_Players.push_back(new FieldPlayer(this,
                               8,
                               SoPWait::Instance(),
                               SoPGlobalPlayerState::Instance(),
                               Vector2D(0,1),
                               Vector2D(0.0, 0.0),
                               Prm.PlayerMass,
                               Prm.PlayerMaxForce,
                               Prm.PlayerMaxSpeedWithoutBall,
                               Prm.PlayerMaxTurnRate,
                               Prm.PlayerScale,
                               PlayerBase::attacker));


 


        m_Players.push_back(new FieldPlayer(this,
                               3,
                               SoPWait::Instance(),
                               SoPGlobalPlayerState::Instance(),
                               Vector2D(0,1),
                               Vector2D(0.0, 0.0),
                               Prm.PlayerMass,
                               Prm.PlayerMaxForce,
                               Prm.PlayerMaxSpeedWithoutBall,
                               Prm.PlayerMaxTurnRate,
                               Prm.PlayerScale,
                               PlayerBase::defender));


        m_Players.push_back(new FieldPlayer(this,
                               5,
                               SoPWait::Instance(),
                               SoPGlobalPlayerState::Instance(),
                               Vector2D(0,1),
                               Vector2D(0.0, 0.0),
                               Prm.PlayerMass,
                               Prm.PlayerMaxForce,
                               Prm.PlayerMaxSpeedWithoutBall,
                               Prm.PlayerMaxTurnRate,
                               Prm.PlayerScale,
                              PlayerBase::defender));

  }

  else
  {

     //goalkeeper
    m_Players.push_back(new GoalKeeper(this,
                               16,
                               SoPTendGoal::Instance(),
                               SoPGlobalKeeperState::Instance(),
                               Vector2D(0,-1),
                               Vector2D(0.0, 0.0),
                               Prm.PlayerMass,
                               Prm.PlayerMaxForce,
                               Prm.PlayerMaxSpeedWithoutBall,
                               Prm.PlayerMaxTurnRate,
                               Prm.PlayerScale));


    //create the players
    m_Players.push_back(new FieldPlayer(this,
                               9,
                               SoPWait::Instance(),
                               SoPGlobalPlayerState::Instance(),
                               Vector2D(0,-1),
                               Vector2D(0.0, 0.0),
                               Prm.PlayerMass,
                               Prm.PlayerMaxForce,
                               Prm.PlayerMaxSpeedWithoutBall,
                               Prm.PlayerMaxTurnRate,
                               Prm.PlayerScale,
                               PlayerBase::attacker));

    m_Players.push_back(new FieldPlayer(this,
                               11,
                               SoPWait::Instance(),
                               SoPGlobalPlayerState::Instance(),
                               Vector2D(0,-1),
                               Vector2D(0.0, 0.0),
                               Prm.PlayerMass,
                               Prm.PlayerMaxForce,
                               Prm.PlayerMaxSpeedWithoutBall,
                               Prm.PlayerMaxTurnRate,
                               Prm.PlayerScale,
                               PlayerBase::attacker));


 
    m_Players.push_back(new FieldPlayer(this,
                               12,
                               SoPWait::Instance(),
                               SoPGlobalPlayerState::Instance(),
                               Vector2D(0,-1),
                               Vector2D(0.0, 0.0),
                               Prm.PlayerMass,
                               Prm.PlayerMaxForce,
                               Prm.PlayerMaxSpeedWithoutBall,
                               Prm.PlayerMaxTurnRate,
                               Prm.PlayerScale,
                               PlayerBase::defender));


    m_Players.push_back(new FieldPlayer(this,
                               14,
                               SoPWait::Instance(),
                               SoPGlobalPlayerState::Instance(),
                               Vector2D(0,-1),
                               Vector2D(0.0, 0.0),
                               Prm.PlayerMass,
                               Prm.PlayerMaxForce,
                               Prm.PlayerMaxSpeedWithoutBall,
                               Prm.PlayerMaxTurnRate,
                               Prm.PlayerScale,
                               PlayerBase::defender));
                      
  }
}

void SonsOfPitches::prepareForKickoff ()
{
	GetFSM()->ChangeState(SoPPrepareForKickOff::Instance());
}

//---------------------- UpdateTargetsOfWaitingPlayers ------------------------
//
//  
void SonsOfPitches::UpdateTargetsOfWaitingPlayers()const
{
  std::vector<PlayerBase*>::const_iterator it = m_Players.begin();

  for (it; it != m_Players.end(); ++it)
  {  
    if ( (*it)->Role() != PlayerBase::goal_keeper )
    {
      //cast to a field player
      FieldPlayer* plyr = static_cast<FieldPlayer*>(*it);
      
      if ( plyr->GetFSM()->isInState(*SoPWait::Instance()) ||
           plyr->GetFSM()->isInState(*SoPReturnToHomeRegion::Instance()) )
      {
        plyr->Steering()->SetTarget(plyr->HomeRegion()->Center());
      }
    }
  }
}
  //------------------------ CanShoot --------------------------------------
//
//  Given a ball position, a kicking power and a reference to a vector2D
//  this function will sample random positions along the opponent's goal-
//  mouth and check to see if a goal can be scored if the ball was to be
//  kicked in that direction with the given power. If a possible shot is 
//  found, the function will immediately return true, with the target 
//  position stored in the vector ShotTarget.
//------------------------------------------------------------------------
bool SonsOfPitches::CanShoot(Vector2D  BallPos,
                          double     power, 
                          Vector2D& ShotTarget)const        // GT 2/10/10 Override
{
  //the number of randomly created shot targets this method will test
  //GT 2/10/10 the number has been multiplied by three relative to the Buckland team
  int NumAttempts = Prm.NumAttemptsToFindValidStrike*3; 

  while (NumAttempts--)
  {
    //choose a random position along the opponent's goal mouth. (making
    //sure the ball's radius is taken into account)
    ShotTarget = OpponentsGoal()->Center();

    //the y value of the shot position should lay somewhere between two
    //goalposts (taking into consideration the ball diameter)
    int MinYVal = OpponentsGoal()->LeftPost().y + Pitch()->Ball()->BRadius();
    int MaxYVal = OpponentsGoal()->RightPost().y - Pitch()->Ball()->BRadius();

    ShotTarget.y = (double)RandInt(MinYVal, MaxYVal);

    //make sure striking the ball with the given power is enough to drive
    //the ball over the goal line.
    double time = Pitch()->Ball()->TimeToCoverDistance(BallPos,
                                                      ShotTarget,
                                                      power);
    
    //if it is, this shot is then tested to see if any of the opponents
    //can intercept it.
    if (time >= 0)
    {
      if (isPassSafeFromAllOpponents(BallPos, ShotTarget, NULL, power))
      {
        return true;
      }
    }
  }
  
  return false;
}



