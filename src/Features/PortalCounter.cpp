#include "PortalCounter.hpp"

#include "Command.hpp"
#include "Features/Session.hpp"
#include "Features/Speedrun/SpeedrunTimer.hpp"
#include "Features/Timer/Timer.hpp"
#include "Modules/Engine.hpp"
#include "Utils/SDK.hpp"
#include "Variable.hpp"
#include "Features/Hud/Hud.hpp"

#include <cstring>

static const char *MY_EVENTS[] = {
	"portal_player_portaled",
	"portal_fired",
};

PortalCounter *portalCounter;

PortalCounter::PortalCounter()
	: m_bRegisteredForEvents(false),
	orange_placed(0),
	orange_entered(0),
	blue_placed(0),
	blue_entered(0){
	this->hasLoaded = true;
}
void PortalCounter::Init() {
	if (engine->hasLoaded && engine->AddListener && !this->m_bRegisteredForEvents) {
		for (const auto &event : MY_EVENTS) {
			this->m_bRegisteredForEvents = engine->AddListener(engine->s_GameEventManager->ThisPtr(), this, event, true);

			if (this->m_bRegisteredForEvents) {
				//console->DevMsg("SAR: Added event listener for %s!\n", event);
			} else {
				console->DevWarning("SAR: Failed to add event listener for %s!\n", event);
				break;
			}
		}
	}
}

void PortalCounter::Shutdown() {
	if (engine->hasLoaded && engine->RemoveListener && this->m_bRegisteredForEvents) {
		engine->RemoveListener(engine->s_GameEventManager->ThisPtr(), this);
		this->m_bRegisteredForEvents = false;
	}

	/*if (tier1->hasLoaded && tier1->RemoveGlobalChangeCallback && this->installedChangeCallback) {
        tier1->RemoveGlobalChangeCallback(tier1->g_pCVar->ThisPtr(), (FnChangeCallback_t)Listener::OnCheatsChanged);
        this->installedChangeCallback = false;
    }*/
}

PortalCounter::~PortalCounter() {
	this->Shutdown();
}

void PortalCounter::FireGameEvent(IGameEvent *ev) {
	if (!ev) {
		return;
	}

	if (!strcmp(ev->GetName(), "portal_player_portaled")) {
		if (ev->GetBool("portal2")) {
				portalCounter->orange_entered++;
			}

	 	else {
			portalCounter->blue_entered++;
		}
	} else if (!strcmp(ev
		->GetName(), "portal_fired")) {
		if (ev->GetBool("leftportal")) {
					portalCounter->blue_placed++;
				}
			else {
				portalCounter->orange_placed++;
			}
	}
}

HUD_ELEMENT(rowan, "0", "Default example.\n", HudType_InGame | HudType_Paused) {
	ctx->DrawElement("Blue placed: %i", portalCounter->blue_placed);
	ctx->DrawElement("Orange placed: %i", portalCounter->orange_placed);
	ctx->DrawElement("Blue entered: %i", portalCounter->blue_entered);
	ctx->DrawElement("Orange entered: %i", portalCounter->orange_entered);
}
HUD_ELEMENT(rowan_active, "0", "Default example.\n", HudType_InGame | HudType_Paused) {
	ctx->DrawElement("Active: %s", portalCounter->m_bRegisteredForEvents ? "true" : "false");
}

CON_COMMAND(sar_rowan_results, "Useful help description.\n") {
	console->Print("Blue placed: %i\n", portalCounter->blue_placed);
	console->Print("Orange placed: %i\n", portalCounter->orange_placed);
	console->Print("Blue entered: %i\n", portalCounter->blue_entered);
	console->Print("Orange entered: %i\n", portalCounter->orange_entered);
}

CON_COMMAND(sar_rowan_toggle, "Useful help description.\n") {
	if (portalCounter
		->m_bRegisteredForEvents) {
			portalCounter->Shutdown();
	}
	else {
		portalCounter->Init();
	}
	console->Print("toggled %s!\n", portalCounter->m_bRegisteredForEvents ? "on" : "off");
}
int PortalCounter::GetEventDebugID() {
	return 42;
}