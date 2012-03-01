#include "SoPTeamStates.h"
#include "../../AbstSoccerTeam.h"
#include "../../PlayerBase.h"
#include "Messaging/MessageDispatcher.h"
#include "../../SoccerMessages.h"
#include "../../constants.h"
#include "../../SoccerPitch.h"

//uncomment to send state info to debug window
//#define DEBUG_TEAM_STATES
#include "Debug/DebugConsole.h"

//************************************************************************ ATTACKING

SoPAttacking* SoPAttacking::Instance()
{
  static SoPAttacking instance;

  return &instance;
}


void SoPAttacking::Enter(AbstSoccerTeam* team)
{
//#ifdef DEBUG_TEAM_STATES
  debug_con << team->Name() << " entering SoPAttacking state" << "";
//#endif

  //these define the home regions for this state of each of the players
  //This configuration is designed to favor a tight passing pattern with
  //and option to do a long pass to the forward player -MW
  const int BlueRegions[TeamSize] = {1,9,8,3,4};
  const int RedRegions[TeamSize] = {16,8,9,14,13};

  //set up the player's home regions
  if (team->Color() == AbstSoccerTeam::blue)
  {
    team->ChangePlayerHomeRegions(BlueRegions);
  }
  else
  {
    team->ChangePlayerHomeRegions(RedRegions);
  }

  //if a player is in either the Wait or ReturnToHomeRegion states, its
  //steering target must be updated to that of its new home region to enable
  //it to move into the correct position.
  team->UpdateTargetsOfWaitingPlayers();
}


void SoPAttacking::Execute(AbstSoccerTeam* team)
{
  //if this team is no longer in control change states
  if (!team->InControl())
  {
    team->GetFSM()->ChangeState(SoPDefending::Instance()); return;
  }
  
  //calculate the best position for any supporting attacker to move to
  team->DetermineBestSupportingPosition();
}

void SoPAttacking::Exit(AbstSoccerTeam* team)
{
  //there is no supporting player for defense
  team->SetSupportingPlayer(NULL);
}



//************************************************************************ DEFENDING

SoPDefending* SoPDefending::Instance()
{
  static SoPDefending instance;

  return &instance;
}

void SoPDefending::Enter(AbstSoccerTeam* team)
{
#ifdef DEBUG_TEAM_STATES
  debug_con << team->Name() << " entering SoPDefending state" << "";
#endif

  //these define the home regions for this state of each of the players
  const int BlueRegions[TeamSize] = {1,6,8,7,4};
  const int RedRegions[TeamSize] = {16,9,11,10,13};

  //set up the player's home regions
  if (team->Color() == AbstSoccerTeam::blue)
  {
    team->ChangePlayerHomeRegions(BlueRegions);
  }
  else
  {
    team->ChangePlayerHomeRegions(RedRegions);
  }
  
  //if a player is in either the Wait or ReturnToHomeRegion states, its
  //steering target must be updated to that of its new home region
  team->UpdateTargetsOfWaitingPlayers();
}

void SoPDefending::Execute(AbstSoccerTeam* team)
{
  //if in control change states
  if (team->InControl())
  {
    team->GetFSM()->ChangeState(SoPAttacking::Instance()); return;
  }
}


void SoPDefending::Exit(AbstSoccerTeam* team)
{
#ifdef DEBUG_TEAM_STATES
  debug_con << team->Name() << " exiting SoPDefending state" << "";
#endif
}


//************************************************************************ KICKOFF
SoPPrepareForKickOff* SoPPrepareForKickOff::Instance()
{
  static SoPPrepareForKickOff instance;

  return &instance;
}

void SoPPrepareForKickOff::Enter(AbstSoccerTeam* team)
{
  //reset key player pointers
  team->SetControllingPlayer(NULL);
  team->SetSupportingPlayer(NULL);
  team->SetReceiver(NULL);
  team->SetPlayerClosestToBall(NULL);

  //send Msg_GoHome to each player.
  team->ReturnAllFieldPlayersToHome();
}

void SoPPrepareForKickOff::Execute(AbstSoccerTeam* team)
{
  //if both teams in position, start the game
  if (team->AllPlayersAtHome() && team->Opponents()->AllPlayersAtHome() ||
		(team->AllPlayersAtHome() && team->Pitch()->OneTeamReady()))
  {
    team->GetFSM()->ChangeState(SoPDefending::Instance());
  }
}

void SoPPrepareForKickOff::Exit(AbstSoccerTeam* team)
{
	if (team->Color() == AbstSoccerTeam::blue)
		team->Pitch()->SetBlueTeamReady();
	else
		team->Pitch()->SetRedTeamReady();
}


