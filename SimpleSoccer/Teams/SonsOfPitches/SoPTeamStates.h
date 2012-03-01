#ifndef SOPTEAMSTATES_H
#define SOPTEAMSTATES_H

//------------------------------------------------------------------------
//
//  Name: TeamStates.h
//
//  Desc: State prototypes for soccer team states
//
//  Author: Mat Buckland 2003 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <string>

#include "FSM/State.h"
#include "Messaging/Telegram.h"
#include "../../constants.h"


class AbstSoccerTeam;


//------------------------------------------------------------------------
class SoPAttacking : public State<AbstSoccerTeam>
{ 
private:
  
  SoPAttacking(){}

public:

  //this is a singleton
  static SoPAttacking* Instance();

  void Enter(AbstSoccerTeam* team);

  void Execute(AbstSoccerTeam* team);

  void Exit(AbstSoccerTeam* team);

  bool OnMessage(AbstSoccerTeam*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class SoPDefending : public State<AbstSoccerTeam>
{ 
private:
  
  SoPDefending(){}

public:

    //this is a singleton
  static SoPDefending* Instance();

  void Enter(AbstSoccerTeam* team);

  void Execute(AbstSoccerTeam* team);

  void Exit(AbstSoccerTeam* team);

  bool OnMessage(AbstSoccerTeam*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class SoPPrepareForKickOff : public State<AbstSoccerTeam>
{ 
private:
  
  SoPPrepareForKickOff(){}

public:

    //this is a singleton
  static SoPPrepareForKickOff* Instance();
  
  void Enter(AbstSoccerTeam* team);

  void Execute(AbstSoccerTeam* team);

  void Exit(AbstSoccerTeam* team);

  bool OnMessage(AbstSoccerTeam*, const Telegram&){return false;}
};


#endif