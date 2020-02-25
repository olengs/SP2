#pragma once
#include "CNode.h"
class CItemList
{
private:
	CNode* head, *tail;
public:
	CItemList();
	~CItemList();
	void addItem(TRS& trs);
	void addItem();
	void addItem(Vector3 translate);
	void removeItem(CNode* current);
	TRS& getTRS(CNode* current);
	CNode* gethead();
	CNode* gettail();
};

