#pragma once
#include <TRS.h>
class CNode
{
private:
	CNode* next, * prev;
public:
	TRS transformation;
	CNode(TRS& trs);
	CNode();
	CNode* getnext();
	CNode* getprev();
	void setprev(CNode* newprev);
	void setnext(CNode* newnext);
	
};

