#pragma once

#include <vector>
#include <Trackable.h>

class Node;

class Path:public Trackable
{
public:
	Path();
	~Path();

	Node* peekNode( int index ) const;
	Node* peekNextNode() const;
	Node* getAndRemoveNextNode();
	bool containsNode( Node* pNode );
	int getNumNodes() const { 
		if (!mNodes.empty())
			return mNodes.size();
		else
			return 0;
	
	};

	void addNode( Node* pNode );

private:
	std::vector<Node*> mNodes;

};