/*
 *  EngineState.h
 *  TractionEdge
 *
 *  Created by Steven Hamilton on 19/11/09.
 *  Copyright 2009 scorch.org. All rights reserved.
 *
 */

#ifndef __ENGINESTATE_H__
#define __ENGINESTATE_H__

#include "Engine.h"

class EngineState{
public:
	virtual ~EngineState(){};
	virtual void enter(Engine*){};
	virtual void execute(Engine*){};
	virtual void exit(Engine*){};
    screenType_t screenType;
};

class EngineStateOptions : public EngineState{
private:
	EngineStateOptions(const EngineStateOptions &);
	EngineStateOptions& operator=(const EngineStateOptions &);
public:
	EngineStateOptions(){}
	~EngineStateOptions(){}
	//methods
	static EngineStateOptions* instance();
	virtual void enter(Engine*);
	virtual void execute(Engine*);
	virtual void exit(Engine*);
};

class EngineStateMatch : public EngineState{
private:
	EngineStateMatch(const EngineStateMatch &);
	EngineStateMatch& operator=(const EngineStateMatch &);
public:
	EngineStateMatch(){}
	~EngineStateMatch(){}
	//methods
	static EngineStateMatch* instance();
	virtual void enter(Engine*);
	virtual void execute(Engine*);
	virtual void exit(Engine*);
};


class EngineStateTitle : public EngineState{
private:
	EngineStateTitle(const EngineStateTitle &);
	EngineStateTitle& operator=(const EngineStateTitle &);
public:
	EngineStateTitle(){}
	~EngineStateTitle(){}
	//methods
	static EngineStateTitle* instance();
	virtual void enter(Engine*);
	virtual void execute(Engine*);
	virtual void exit(Engine*);
};


class EngineStateHelp : public EngineState{
private:
	EngineStateHelp(const EngineStateHelp &);
	EngineStateHelp& operator=(const EngineStateHelp &);
public:
	EngineStateHelp(){}
	~EngineStateHelp(){}
	//methods
	static EngineStateHelp* instance();
	virtual void enter(Engine*);
	virtual void execute(Engine*);
	virtual void exit(Engine*);
};

class EngineStateSquad : public EngineState{
private:
	EngineStateSquad(const EngineStateSquad &);
	EngineStateSquad& operator=(const EngineStateSquad &);
public:
	EngineStateSquad(){}
	~EngineStateSquad(){}
	//methods
	static EngineStateSquad* instance();
	virtual void enter(Engine*);
	virtual void execute(Engine*);
	virtual void exit(Engine*);
};

class EngineStateIntro : public EngineState{
private:
	EngineStateIntro(const EngineStateIntro &);
	EngineStateIntro& operator=(const EngineStateIntro &);
public:
	EngineStateIntro(){}
	~EngineStateIntro(){}
	//methods
	static EngineStateIntro* instance();
	virtual void enter(Engine*);
	virtual void execute(Engine*);
	virtual void exit(Engine*);
};

class EngineStateResult : public EngineState{
private:
	EngineStateResult(const EngineStateResult &);
	EngineStateResult& operator=(const EngineStateResult &);
public:
	EngineStateResult(){}
	~EngineStateResult(){}
	//methods
	static EngineStateResult* instance();
	virtual void enter(Engine*);
	virtual void execute(Engine*);
	virtual void exit(Engine*);
};

class EngineStateGameOver : public EngineState{
private:
	EngineStateGameOver(const EngineStateGameOver &);
	EngineStateGameOver& operator=(const EngineStateGameOver &);
public:
	EngineStateGameOver(){}
	~EngineStateGameOver(){}
	//methods
	static EngineStateGameOver* instance();
	virtual void enter(Engine*);
	virtual void execute(Engine*);
	virtual void exit(Engine*);
};
#endif
