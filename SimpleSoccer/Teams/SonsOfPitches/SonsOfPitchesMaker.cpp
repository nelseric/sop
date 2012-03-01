#include "SonsOfPitchesMaker.h"


const SonsOfPitchesMaker SonsOfPitchesMaker::registerThis;

SonsOfPitchesMaker::SonsOfPitchesMaker() : TeamMaker("SonsOfPitches") {}

AbstSoccerTeam* SonsOfPitchesMaker::makeTeam(Goal* home_goal,
             Goal*        opponents_goal,
             SoccerPitch* pitch,
			 AbstSoccerTeam::team_color   color) const
   { 
      return new SonsOfPitches(home_goal, opponents_goal, pitch, color); 
   }