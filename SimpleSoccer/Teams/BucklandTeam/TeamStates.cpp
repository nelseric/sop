#include "TeamStates.h"
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

Attacking* Attacking::Instance()
{
  static Attacking instance;

  return &instance;
}


void Attacking::Enter(AbstSoccerTeam* team)
{
//#ifdef DEBUG_TEAM_STATES
  debug_con << team->Name() << " entering Attacking state" << "";
//#endif

  //these define the home regions for this state of each of the players
  const int BlueRegions[TeamSize] = {1,12,14,6,4};
  const int RedRegions[TeamSize] = {16,3,5,9,13};

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


void Attacking::Execute(AbstSoccerTeam* team)
{
  //if this team is no longer in control change states
  if (!team->InControl())
  {
    team->GetFSM()->ChangeState(Defending::Instance()); return;
  }

  //calculate the best position for any supporting attacker to move to
  team->DetermineBestSupportingPosition();
}

void Attacking::Exit(AbstSoccerTeam* team)
{
  //there is no supporting player for defense
  team->SetSupportingPlayer(NULL);
}



//************************************************************************ DEFENDING

Defending* Defending::Instance()
{
  static Defending instance;

  return &instance;
}

void Defending::Enter(AbstSoccerTeam* team)
{
#ifdef DEBUG_TEAM_STATES
  debug_con << team->Name() << " entering Defending state" << "";
#endif

  //these define the home regions for this state of each of the players
  const int BlueRegions[TeamSize] = {1,6,8,3,5};
  const int RedRegions[TeamSize] = {16,9,11,12,14};

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

void Defending::Execute(AbstSoccerTeam* team)
{
  //if in control change states
  if (team->InControl())
  {
    team->GetFSM()->ChangeState(Attacking::Instance()); return;
  }
}


void Defending::Exit(AbstSoccerTeam* team){}


//************************************************************************ KICKOFF
PrepareForKickOff* PrepareForKickOff::Instance()
{
  static PrepareForKickOff instance;

  return &instance;
}

void PrepareForKickOff::Enter(AbstSoccerTeam* team)
{
  //reset key player pointers
  team->SetControllingPlayer(NULL);
  team->SetSupportingPlayer(NULL);
  team->SetReceiver(NULL);
  team->SetPlayerClosestToBall(NULL);

  //send Msg_GoHome to each player.
  team->ReturnAllFieldPlayersToHome();
}

void PrepareForKickOff::Execute(AbstSoccerTeam* team)
{
  //if both teams in position, start the game
  if (team->AllPlayersAtHome() && team->Opponents()->AllPlayersAtHome() ||
		(team->AllPlayersAtHome() && team->Pitch()->OneTeamReady()))
  {
    team->GetFSM()->ChangeState(Defending::Instance());
  }
}

void PrepareForKickOff::Exit(AbstSoccerTeam* team)
{
	if (team->Color() == AbstSoccerTeam::blue)
		team->Pitch()->SetBlueTeamReady();
	else
		team->Pitch()->SetRedTeamReady();
}


