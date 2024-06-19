#pragma once
#include "Command.hpp"
#include "Feature.hpp"
#include "Utils/SDK.hpp"
#include "Variable.hpp"


class PortalCounter : public IGameEventListener2
	, public Feature {
public:
	bool m_bRegisteredForEvents;
	int orange_placed;
	int orange_entered;
	int blue_placed;
	int blue_entered;

public:
	PortalCounter();
	void Init();
	void Shutdown();

	virtual ~PortalCounter();
	virtual void FireGameEvent(IGameEvent *ev);
	virtual int GetEventDebugID();
};

extern PortalCounter *portalCounter;

extern Command sar_rowan_results;
