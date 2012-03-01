#ifndef SONSOFPITCHESMAKER_H
#define SONSOFPITCHESMAKER_H
#pragma warning (disable:4786)

//------------------------------------------------------------------------
//
//  Name:   MySoccerTeamMaker.h
//
//  Desc:   Factory class for SoccerTeam. 
//
//  Author: Robin Burke 2005 rburke@cs.depaul.edu
//
//------------------------------------------------------------------------

#include "../../TeamMaker.h"
#include "SonsOfPitches.h"
#include <string>
#include "../../Goal.h"
#include "../../SoccerPitch.h"

class SonsOfPitchesMaker : public TeamMaker {
private:
   SonsOfPitchesMaker();

   AbstSoccerTeam* makeTeam(Goal*        home_goal,
             Goal*        opponents_goal,
             SoccerPitch* pitch,
			 AbstSoccerTeam::team_color   color) const;
	
   // creation of static instance does automatic registration
    static const SonsOfPitchesMaker registerThis;
};
#endif