#ifndef SOPKEEPERSTATES_H
#define SOPKEEPERSTATES_H
//------------------------------------------------------------------------
//
//  Name: SoPGoalKeeperStates.h
//
//  Desc:   Declarations of all the states used by a Simple Soccer
//          goalkeeper
//
//  Author: Mat Buckland 2003 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <string>
#include "FSM/State.h"
#include "Messaging/Telegram.h"
#include "../../constants.h"


class GoalKeeper;
class SoccerPitch;


class SoPGlobalKeeperState: public State<GoalKeeper>
{
private:
  
  SoPGlobalKeeperState(){}

public:

  //this is a singleton
  static SoPGlobalKeeperState* Instance();

  void Enter(GoalKeeper* keeper){}

  void Execute(GoalKeeper* keeper){}

  void Exit(GoalKeeper* keeper){}

  bool OnMessage(GoalKeeper*, const Telegram&);
};

//-----------------------------------------------------------------------------

class SoPTendGoal: public State<GoalKeeper>
{
private:
  
  SoPTendGoal(){}

public:

  //this is a singleton
  static SoPTendGoal* Instance();

  void Enter(GoalKeeper* keeper);

  void Execute(GoalKeeper* keeper);

  void Exit(GoalKeeper* keeper);

  bool OnMessage(GoalKeeper*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class SoPInterceptBall: public State<GoalKeeper>
{
private:
  
  SoPInterceptBall(){}

public:

  //this is a singleton
  static SoPInterceptBall* Instance();

  void Enter(GoalKeeper* keeper);

  void Execute(GoalKeeper* keeper);

  void Exit(GoalKeeper* keeper);

  bool OnMessage(GoalKeeper*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class SoPReturnHome: public State<GoalKeeper>
{
private:
  
  SoPReturnHome(){}

public:

  //this is a singleton
  static SoPReturnHome* Instance();

  void Enter(GoalKeeper* keeper);

  void Execute(GoalKeeper* keeper);

  void Exit(GoalKeeper* keeper);

  bool OnMessage(GoalKeeper*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class SoPPutBallBackInPlay: public State<GoalKeeper>
{
private:
  
  SoPPutBallBackInPlay(){}

public:

  //this is a singleton
  static SoPPutBallBackInPlay* Instance();

  void Enter(GoalKeeper* keeper);

  void Execute(GoalKeeper* keeper);

  void Exit(GoalKeeper* keeper){}

  bool OnMessage(GoalKeeper*, const Telegram&){return false;}
};





#endif