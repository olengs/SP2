#include "CItemList.h"

CItemList::CItemList()
{
	this->head = this->tail = nullptr;
}

CItemList::~CItemList()
{
	for (CNode* current = head; current != nullptr;) {
		current = current->getnext();
		delete current->getprev();
	}
}

void CItemList::addItem(TRS& trs)
{
	CNode* newNode = new CNode(trs);
	if (head == nullptr) {
		head = tail = newNode;
	}
	else {
		tail->setnext(newNode);
		newNode->setprev(tail);
		tail = newNode;
	}
}

void CItemList::addItem()
{
	TRS trs;
	CNode* newNode = new CNode(trs);
	if (head == nullptr) {
		head = tail = newNode;
	}
	else {
		tail->setnext(newNode);
		newNode->setprev(tail);
		tail = newNode;
	}
}

void CItemList::addItem(Vector3 translate)
{
	TRS trs;
	trs.translate = translate;
	CNode* newNode = new CNode(trs);
	if (head == nullptr) {
		head = tail = newNode;
	}
	else {
		tail->setnext(newNode);
		newNode->setprev(tail);
		tail = newNode;
	}
}

void CItemList::removeItem(CNode* current)
{
	current->getprev()->setnext(current->getnext());
	current->getnext()->setprev(current->getprev());
	delete current;
}

TRS& CItemList::getTRS(CNode* current)
{
	return current->transformation;
}

CNode* CItemList::gethead()
{
	return this->head;
}

CNode* CItemList::gettail()
{
	return this->tail;
}
