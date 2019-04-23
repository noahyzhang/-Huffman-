#include<vector>
#include<queue>


template<class T>
struct HuffmanTreeNode
{
	HuffmanTreeNode(const T& weight) 
		: _weight(weight)
		, _pParent(nullptr)
		, _pLeft(nullptr)
		, _pRight(nullptr)
	{}

	HuffmanTreeNode<T>* _pLeft;
	HuffmanTreeNode<T>* _pRight;
	HuffmanTreeNode<T>* _pParent;
	T _weight;
};

template<class T>
class MyCompare {
	typedef HuffmanTreeNode<T>* PNode;
	bool reverse;
public:
	MyCompare(const bool& revparam = false)
		: reverse(revparam)
	{}
	bool operator()(const PNode& left, const PNode& right) const
	{
		if (reverse)
			return left->_weight > right->_weight;
		else
			return right->_weight < left->_weight;
	}
};

template<class T>
class HuffmanTree {
	typedef HuffmanTreeNode<T> Node;
	typedef Node* PNode;
public:
	HuffmanTree()
		: _pRoot(nullptr)
	{}
	~HuffmanTree()
	{
		Destory(_pRoot);
	}
	void CreateHuffmanTree(const std::vector<T>& vec, const T& invalid)
	{
		if (vec.empty())
			return; 
		std::priority_queue<PNode, std::vector<PNode>, MyCompare<T>> q(MyCompare<T>(true));
		for (size_t i = 0; i < vec.size(); ++i)
		{
			if(vec[i] != invalid)
				q.push(new Node(vec[i]));  //这块必须给一个对象，传的参数是const类型
		}
		while (q.size() > 1)
		{
			PNode pLeft = q.top();
			q.pop();
			PNode pRight = q.top();
			q.pop();
			PNode pParent = new Node(pLeft->_weight + pRight->_weight);
			pParent->_pLeft = pLeft;
			pParent->_pRight = pRight;
			pLeft->_pParent = pParent;
			pRight->_pParent = pParent;
			q.push(pParent);
		}
		_pRoot = q.top();
	}
	PNode GetRoot()
	{
		return _pRoot;
	}
private:
	void Destory(PNode& pRoot)
	{
		if (pRoot != nullptr)
		{
			Destory(pRoot->_pLeft);
			Destory(pRoot->_pRight);
			delete pRoot;
			pRoot = nullptr;
		}
	}
private:
	HuffmanTreeNode<T>* _pRoot;
};
