#ifndef SONSOFPITCHES_H
#define SONSOFPITCHES_H
#pragma warning (disable:4786)

//------------------------------------------------------------------------
//
//  Name:   SonsOfPitches.h
//
//  Desc:   class to define a team of soccer playing agents. A SoccerTeam
//          contains several field players and one goalkeeper. A SoccerTeam
//          is implemented as a finite state machine and has states for
//          attacking, defending, and KickOff.
//
//  Author: Eric Nelson
//
//------------------------------------------------------------------------

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




                
class SonsOfPitches : public AbstSoccerTeam
{

public:

  SonsOfPitches(Goal*        home_goal,
             Goal*        opponents_goal,
             SoccerPitch* pitch,
             team_color   color);

  ~SonsOfPitches();

  std::string Name()const{return "Sons of Pitches";}

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