#ifndef MYSOCCERTEAM_H
#define MYSOCCERTEAM_H
#pragma warning (disable:4786)

//------------------------------------------------------------------------
//
//  Name:   MySoccerTeam.h
//
//  Desc:   class to define a team of soccer playing agents. A SoccerTeam
//          contains several field players and one goalkeeper. A SoccerTeam
//          is implemented as a finite state machine and has states for
//          attacking, defending, and KickOff.
//
//  Author: Mat Buckland 2003 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
// 2/3/2006 RB
// MySoccerTeam
// Modified as an example of a custom soccer team.

#include <vector>

#include "Game/Region.h"
#include "../../SupportSpotCalculator.h"
#include "FSM/StateMachine.h"
#include "../../AbstSoccerTeam.h"

                
class Goal;
class PlayerBase;
class FieldPlayer;
class SoccerPitch;
class GoalKeeper;
class SupportSpotCalculator;




                
class MySoccerTeam : public AbstSoccerTeam
{

public:

  MySoccerTeam(Goal*        home_goal,
             Goal*        opponents_goal,
             SoccerPitch* pitch,
             team_color   color);

  ~MySoccerTeam();

  std::string Name()const{return "MyTeam";}

	void InitStateMachine();
	void CreatePlayers();
	void InitPlayers();
	void prepareForKickoff();
	
  void UpdateTargetsOfWaitingPlayers()const;
  bool CanShoot(Vector2D  BallPos,
                          double     power, 
                          Vector2D& ShotTarget)const;   // GT 2/10/10 CanShoot override
};

#endif