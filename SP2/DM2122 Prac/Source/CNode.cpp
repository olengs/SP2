#include "CNode.h"

CNode::CNode(TRS& trs)
{
	this->next = this->prev = nullptr;
	this->transformation = trs;
}

CNode::CNode()
{
	this->next = this->prev = nullptr;
}

CNode* CNode::getnext()
{
	return this->next;
}

CNode* CNode::getprev()
{
	return this->prev;
}

void CNode::setprev(CNode* newprev)
{
	this->prev = newprev;
}

void CNode::setnext(CNode* newnext)
{
	this->next = newnext;
}
