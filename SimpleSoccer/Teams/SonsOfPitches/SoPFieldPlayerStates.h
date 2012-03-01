#ifndef SOPFIELDPLAYERSTATES_H
#define SOPFIELDPLAYERSTATES_H
//------------------------------------------------------------------------
//
//  Name: FieldPlayerStates.h
//
//  Desc: States for the field players of Simple Soccer. See my book
//        for detailed descriptions
//
//  Author: Mat Buckland 2003 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------

#include <string>

#include "FSM/State.h"
#include "Messaging/Telegram.h"
#include "../../constants.h"


class FieldPlayer;
class SoccerPitch;


//------------------------------------------------------------------------
class SoPGlobalPlayerState : public State<FieldPlayer>
{
private:
  
  SoPGlobalPlayerState(){}

public:

  //this is a singleton
  static SoPGlobalPlayerState* Instance();

  void Enter(FieldPlayer* player){}

  void Execute(FieldPlayer* player);

  void Exit(FieldPlayer* player){}

  bool OnMessage(FieldPlayer*, const Telegram&);
};

//------------------------------------------------------------------------
class SoPChaseBall : public State<FieldPlayer>
{
private:
  
  SoPChaseBall(){}

public:

  //this is a singleton
  static SoPChaseBall* Instance();

  void Enter(FieldPlayer* player);

  void Execute(FieldPlayer* player);

  void Exit(FieldPlayer* player);

  bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class SoPDribble : public State<FieldPlayer>
{
private:
  
  SoPDribble(){}

public:

  //this is a singleton
  static SoPDribble* Instance();

  void Enter(FieldPlayer* player);

  void Execute(FieldPlayer* player);

  void Exit(FieldPlayer* player){}

  bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};


//------------------------------------------------------------------------
class SoPReturnToHomeRegion: public State<FieldPlayer>
{
private:
  
  SoPReturnToHomeRegion(){}

public:

  //this is a singleton
  static SoPReturnToHomeRegion* Instance();

  void Enter(FieldPlayer* player);

  void Execute(FieldPlayer* player);

  void Exit(FieldPlayer* player);

  bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class SoPWait: public State<FieldPlayer>
{
private:
  
  SoPWait(){}

public:

  //this is a singleton
  static SoPWait* Instance();

  void Enter(FieldPlayer* player);

  void Execute(FieldPlayer* player);

  void Exit(FieldPlayer* player);

  bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class SoPKickBall: public State<FieldPlayer>
{
private:
  
  SoPKickBall(){}

public:

  //this is a singleton
  static SoPKickBall* Instance();

  void Enter(FieldPlayer* player);

  void Execute(FieldPlayer* player);

  void Exit(FieldPlayer* player){}

  bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

//------------------------------------------------------------------------
class SoPReceiveBall: public State<FieldPlayer>
{
private:
  
  SoPReceiveBall(){}

public:

  //this is a singleton
  static SoPReceiveBall* Instance();

  void Enter(FieldPlayer* player);

  void Execute(FieldPlayer* player);

  void Exit(FieldPlayer* player);

  bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};


//------------------------------------------------------------------------
class SoPSupportAttacker: public State<FieldPlayer>
{
private:
  
  SoPSupportAttacker(){}

public:

  //this is a singleton
  static SoPSupportAttacker* Instance();

  void Enter(FieldPlayer* player);

  void Execute(FieldPlayer* player);

  void Exit(FieldPlayer* player);

  bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};




  
#endif