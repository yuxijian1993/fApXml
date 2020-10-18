#pragma once

#include "fApFile.hpp"

using namespace std;

enum CurState
{
	READ_NORMAL,			//正常读取
	USELESS_BYTE,			//无用的字符
	SAVE_NAME,				//保存节点名
	NAME_OVER,
	//SAVE_ATTRNAME,			//保存属性名
	//ATTRNAME_OVER,			//
	//SAVE_ATTR,				//保存属性内容
	//ATTR_OVER,
	SAVE_TEXT,				//保存节点内容
	TEXT_OVER
};

//节点单个属性
class Attr
{
public:
	Attr() :attrName(""), attrText(""), next(NULL) {};
	void initAttr();

	//节点属性名
	string attrName;
	//节点属性值
	string attrText;
	//下一个属性的指针
	Attr* next;
};

//节点属性链表
class AttrList
{
public:
	//初始化
	AttrList() :head(NULL), latest(NULL), work(NULL), attrNum(0) {};
	//获取头结点
	Attr* getHead();
	//添加属性节点
	BOOL addAttrNode(Attr* an);
	//通过属性名查找属性内容
	Attr* searchAttr(const string& attrname);
	//通过属性名删除属性节点
	BOOL deleteAttr(const string& attrname);
	//清空属性链表
	BOOL clearAttr();
	//	显示or输出
	BOOL showAllattr();

private:
	//第一个属性
	Attr* head;
	//最后一个属性
	Attr* latest;
	//工作指针
	Attr* work;
	//属性链表节点个数
	INT attrNum;
};

//树节点
class xmlNode
{
public:
	xmlNode();


	//节点名
	string nodeName;
	//节点内容
	string nodeText;
	//节点属性链表
	AttrList* nodeAttr;
	//第一个子节点
	xmlNode* firstchild;
	//右兄弟
	xmlNode* rightsibling;
	//该节点是否已经存在过节点结束标志
	BOOL isClosed;
	//节点编号,解决重名节点无法查找的问题
	INT index;
};

//队列节点
class Queuenode
{
public:
	Queuenode() :node(NULL), next(NULL) {};

	xmlNode* node;
	Queuenode* next;
};

//双亲队列
class Queue
{
public:
	Queue() :head(NULL), latest(NULL), work(NULL) {};
	//尾插入
	BOOL QueueInsert(xmlNode* cur);
	//头删除
	BOOL QueueDelelte();
	//清空
	BOOL clearQueue();
	//遍历
	BOOL isInQueue(xmlNode* node);
	//显示or输出
	BOOL showAll();
	//获取队列头
	Queuenode* getHead();


	Queuenode* head;
	Queuenode* latest;
	Queuenode* work;
};

//树
//存储所有的兄弟节点
class TreeNode
{
public:
	TreeNode();
	//添加兄弟
	BOOL addSibling(const string& name, const string& text = "");
	//添加子节点
	BOOL addChild(const string& name, const string& text = "");
	//添加树结构节点--内部判断添加兄弟节点或子节点
	BOOL addTreeNode(const string& name, const string& text = "");
	//遍历整个节点链表
	xmlNode* searchTree(const string& name, const INT& i);
	//判断是否是节点结束标志
	BOOL isNodeOver(const string& name);
	//通过节点结束标志设置节点结束状态
	BOOL setNodeOver(const string& name);
	//给之前创建的节点添加属性
	BOOL setLastNodeAttribute(Attr* attr);
	//给之前创建的节点添加节点内容
	BOOL setLastNodeText(const string& text);
	//获取所有有子节点的节点
	Queue* getParentQueue();
	



private:
	xmlNode* root;
	xmlNode* latest;
	xmlNode* work;

	//保存所有的parent节点
	Queue* parent;
	//兄弟个数
	INT sibNum;
};

class fApXml
{
public:
	//初始化
	fApXml();
	//过滤字符判断
	BOOL filterChar(const char& c);
	//字符串拆分
	//拆分从字符串开始到over之前的所有字符
	//并且删除原字符串中拆分出来的内容,传入的字符也会从原字符串中删除
	string splitWord(const char& end, string& res);
	//拆分从一个字符到另一个字符之间的字符串
	//并且删除原字符串中拆分出来的内容,传入的字符也会从原字符串中删除
	string splitWord(const char& begin, const char& end, string& res);
	//判断是否是xml注释
	BOOL isXMLnote(const string& res);


	//xml内容转树结构
	BOOL Xml2Tree();
	//save
	BOOL save2Txt(const string& filename = "out");


//private:
	fApFile* xml;
	TreeNode* allNode;
};

//		class Attr 方法实现

void Attr::initAttr()
{
	attrName = "";
	attrText = "";
	next = NULL;
}

//		class AttrList 方法实现

//获取头结点
inline Attr* AttrList::getHead()
{
	return head;
}

//添加属性节点
inline BOOL AttrList::addAttrNode(Attr* an)
{
	if (!head)
	{
		latest = new Attr();
		latest->attrName.assign(an->attrName);
		latest->attrText.assign(an->attrText);
		head = latest;
		attrNum = 1;

		return TRUE;
	}
	else
	{
		latest->next = new Attr();
		latest = latest->next;
		latest->attrName.assign(an->attrName);
		latest->attrText.assign(an->attrText);
		attrNum++;

		return TRUE;
	}

	return FALSE;
}

//通过属性名查找属性内容
inline Attr* AttrList::searchAttr(const string& attrname)
{
	if (!head) return NULL;
	work = head;
	while (work)
	{
		if (work->attrName == attrname)
		{
			return work;
		}
		work = work->next;
	}

	return FALSE;
}

//通过属性名删除属性节点
inline BOOL AttrList::deleteAttr(const string& attrname)
{
	if (!head) return TRUE;

	if (latest->attrName == attrname)
	{
		work = head;
		while (work->next != latest)
		{
			work = work->next;
		}
		delete latest;
		latest = work;
		work = NULL;
		attrNum--;

		return TRUE;
	}

	work = head;
	while (work != latest)
	{
		if (work->next->attrName == attrname)
		{
			Attr* temp = work->next;
			work->next = temp->next;
			work = temp;
			temp = NULL;
			delete work;
			work = NULL;
			attrNum--;

			return TRUE;
		}
		work = work->next;
	}

	return FALSE;
}

//清空属性链表
inline BOOL AttrList::clearAttr()
{
	if (!head) return TRUE;

	while (attrNum != 0)
	{
		work = head->next;
		delete head;
		head = work;
		attrNum--;
	}
	latest = NULL;

	return TRUE;
}

inline BOOL AttrList::showAllattr()
{
	if (!head) return FALSE;

	work = head;
	while (work)
	{
		cout << work->attrName << ":";
		cout << work->attrText << " ";
		work = work->next;
	}

	return TRUE;
}

//		class xmlNode 方法实现

xmlNode::xmlNode()
{
	nodeName = "";
	nodeText = "";
	nodeAttr = new AttrList();
	firstchild = NULL;
	rightsibling = NULL;
	isClosed = FALSE;
}

//		class Queue 方法实现

//尾插入
inline BOOL Queue::QueueInsert(xmlNode* cur)
{
	if (!head)
	{
		latest = new Queuenode();
		latest->node = cur;
		head = latest;

		return TRUE;
	}
	else
	{
		latest->next = new Queuenode();
		latest = latest->next;
		latest->node = cur;

		return TRUE;
	}

	return FALSE;
}

//头删除
inline BOOL Queue::QueueDelelte()
{
	if (!head) return FALSE;
	if (!head->next)
	{
		delete latest;
		latest = NULL;
		head = NULL;
		work = NULL;

		return TRUE;
	}

	work = head->next;
	delete head;
	head = work;

	return TRUE;
}

//清空
inline BOOL Queue::clearQueue()
{
	if (!head) return FALSE;
	if (!head->next)
	{
		delete latest;
		latest = NULL;
		head = NULL;
		work = NULL;

		return TRUE;
	}
	else
	{
		while (head)
		{
			work = head->next;
			delete head;
			head = work;
		}
		latest = NULL;

		return TRUE;
	}

	return FALSE;
}

inline BOOL Queue::isInQueue(xmlNode* node)
{
	if (!head) return FALSE;

	work = head;
	while (work)
	{
		if (work->node == node)
		{
			return TRUE;
		}
		work = work->next;
	}

	return FALSE;
}

inline BOOL Queue::showAll()
{
	if (!head) return FALSE;

	work = head;
	while (work)
	{
		cout << "节点名: " << work->node->nodeName << "\n";
		cout << "属性: ";
		work->node->nodeAttr->showAllattr();
		cout << endl;
		cout << "节点内容: " << work->node->nodeText;
		cout << endl;

		work = work->next;
	}

	return TRUE;
}

inline Queuenode* Queue::getHead()
{
	return head;
}

//class TrreNode 方法实现

TreeNode::TreeNode()
{
	root = NULL;
	latest = NULL;
	work = NULL;
	parent = new Queue();
	sibNum = 0;
}

//添加兄弟
BOOL TreeNode::addSibling(const string& name, const string& text)
{
	//根节点不存在(即空树)不可以直接点加兄弟节点
	//如果树只有根节点,则只能在根节点上添加子节点
	if (!root || !root->firstchild) return FALSE;

	work = latest;

	latest->rightsibling = new xmlNode();
	latest = latest->rightsibling;
	latest->nodeName = name;
	latest->nodeText = text;
	if (work->nodeName == latest->nodeName)
	{
		latest->index = work->index++;
	}

	return TRUE;
}

//设置子节点
BOOL TreeNode::addChild(const string& name, const string& text)
{
	//没有根节点,不允许添加子节点
	if (root == NULL) return NULL;
	//添加第一个字节点说明双亲队列中没有当前节点
	//把当前节点扔进双亲队列
	parent->QueueInsert(latest);

	latest->firstchild = new xmlNode();
	latest = latest->firstchild;
	latest->nodeName = name;
	latest->nodeText = text;

	return TRUE;
}

inline BOOL TreeNode::addTreeNode(const string& name, const string& text /*= ""*/)
{
	BOOL isSucceed = FALSE;

	if (!root)
	{
		latest = new xmlNode();
		root = latest;
		root->nodeName = name;
		root->nodeText = text;

		return TRUE;
	}
	else if (latest == root)
	{
		isSucceed = addChild(name, text);
	}
	else if (!latest->isClosed)
	{
		isSucceed = addChild(name, text);
	}
	else if (latest->isClosed)
	{
		isSucceed = addSibling(name, text);
	}

	return isSucceed;
}

inline xmlNode* TreeNode::searchTree(const string& name, const INT& i)
{
	if (!root) return NULL;

	Queuenode* pa = parent->head;
	xmlNode* child = NULL;
	while (pa)
	{
		child = pa->node;
		while (child)
		{
			if (child->nodeName == name && child->index == i)
			{
				return child;
			}
			child = child->firstchild;
		}
		pa = pa->next;
	}

	return NULL;
}

inline BOOL TreeNode::isNodeOver(const string& name)
{
	return latest->isClosed;
}

inline BOOL TreeNode::setNodeOver(const string& name)
{
	if (!root) return TRUE;

	if (latest->isClosed == TRUE) return TRUE;

	if (latest->nodeName == name) latest->isClosed = TRUE;

	return latest->isClosed;
}

inline BOOL TreeNode::setLastNodeAttribute(Attr* attr)
{
	if (!latest) return FALSE;

	latest->nodeAttr->addAttrNode(attr);
	return TRUE;
}

inline BOOL TreeNode::setLastNodeText(const string& text)
{
	if (!latest) return FALSE;

	latest->nodeText.assign(text);
}

inline Queue* TreeNode::getParentQueue()
{
	return parent;
}

//		class fApXml 方法实现

fApXml::fApXml()
{
	xml = new fApFile();
	allNode = new TreeNode();
}

inline BOOL fApXml::filterChar(const char& c)
{
	if (c == '\t' || c == '\n' || c == '\r' || c == '\0' || c == '?')
	{
		return TRUE;
	}

	return FALSE;
}

inline string fApXml::splitWord(const char& end, string& res)
{
	string result = "";

	if (res.find(end) == res.npos) return result;

	INT pos = res.find(end);
	
	result.assign(res, 0, pos);
	res.erase(0, pos + 1);

	return result;
}

inline string fApXml::splitWord(const char& begin, const char& end, string& res)
{
	if (res.find(end) == res.npos) return "";

	INT start = -1;
	INT over = -1;

	for (UINT i = 0; i < res.size(); i++)
	{
		if (res[i] == begin && start == -1)
		{
			start = i;
		}
		else if (res[i] == end && over == -1)
		{
			over = i;
			break;
		}
	}
	//start"string"beginend
	string result;
	result.assign(res, start + 1, over - (start + 1));
	res.erase(0, over + 1);

	return result;
}

inline BOOL fApXml::isXMLnote(const string& res)
{
	if (res[0] == '!' && res[1] == '-' && res[2] == '-') return TRUE;

	return FALSE;
}

inline BOOL fApXml::Xml2Tree()
{
	//临时保存读取的字符串
	string word = "";
	//当前读取状态初始化未正常读取状态
	INT state = READ_NORMAL;
	//临时保存属性
	Attr* tempAttr = new Attr();
	//当前读取的字符
	CHAR byte = 0;
	//保存之前读取的字符
	CHAR prevChar = 0;
	
	string content = xml->getContent();

	INT xmlHeadStart = content.find('<');
	INT xmlHeadEnd = content.find('>');
	content.erase(xmlHeadStart, xmlHeadEnd + 1);

	/*string content;
	if (xml->readType == TYPE_UTF8)
	{
		content = xml->utf2gbk(resource);
	}
	else
	{
		content = resource;
	}*/

	for (UINT i = 0; i < content.size(); i++)
	{
		
		byte = content[i];

		if (filterChar(byte)) continue;

		if (prevChar == '<' && state == READ_NORMAL)
		{
			state = SAVE_NAME;
			goto __WORK_BY_STATE;
		}
		else if (state == SAVE_NAME && byte == '>')
		{
			state = NAME_OVER;
			goto __WORK_BY_STATE;
		}

		if (prevChar == '>' && byte != '<' && state == READ_NORMAL)
		{
			state = SAVE_TEXT;
			goto __WORK_BY_STATE;
		}
		else if (byte == '<' && state == SAVE_TEXT)
		{
			state = TEXT_OVER;
			goto __WORK_BY_STATE;
		}

__WORK_BY_STATE:
		switch (state)
		{
		//状态为保存内容时,把读取的字符保存到字符串中
		case SAVE_NAME:
		//case SAVE_ATTRNAME:
		//case SAVE_ATTR:
		case SAVE_TEXT:
			word.append(1, byte);
			break;
		//节点名保存完毕
		case NAME_OVER:

			if (isXMLnote(word))
			{
				word = "";
				state = READ_NORMAL;
				break;
			}

			if (word.find('/') != word.npos)
			{
				UINT pos = word.find('/');
				if (pos == 0)
				{
					string res(word, 1);
					if (!allNode->isNodeOver(res))
					{
						allNode->setNodeOver(res);
						word = "";
						state = READ_NORMAL;
						break;
					}
					else
					{
						Queue* q = allNode->getParentQueue();
						if (q->latest->node->nodeName == res && !q->latest->node->isClosed)
						{
							q->latest->node->isClosed = TRUE;
							word = "";
							state = READ_NORMAL;
							break;
						}
					}
				}
				else if(word[word.size() - 1] == '/')
				{
					UINT end = word.size() - 1;
					word.erase(end, 1);
					end = word.size() - 1;
					if (word[end] == ' ')
					{
						word.erase(end, 1);
					}
				}
			}

			//如果节点名中存在‘ ’(空格),则说明此节点存在属性
			if (word.find(' ') != word.npos)
			{
				//拆分属性

				//拆分出节点名
				string tempStr = splitWord(' ', word);
				allNode->addTreeNode(tempStr);

				while (word != "")
				{
					tempStr = splitWord('=', word);
					tempAttr->attrName.assign(tempStr);
					tempStr = splitWord('\"', '\"', word);
					tempAttr->attrText.assign(tempStr);
					allNode->setLastNodeAttribute(tempAttr);
				}

				state = READ_NORMAL;
			}
			else
			{
				allNode->addTreeNode(word);
				word = "";
				state = READ_NORMAL;
			}
			
			break;
		//属性名保存完毕
		/*case ATTRNAME_OVER:
			tempAttr->attrName.assign(word);
			word = "";
			state = READ_NORMAL;
			break;
		//属性内容保存完毕
		case ATTR_OVER:
			tempAttr->attrText.assign(word);
			word = "";

			allNode->setLastNodeAttribute(tempAttr);
			tempAttr->initAttr();

			state = READ_NORMAL;
			break;*/
		//节点内容保存完毕
		case TEXT_OVER:
			//tempNode->nodeText.assign(word);
			allNode->setLastNodeText(word);
			word = "";
			state = READ_NORMAL;
			break;
		default:
			break;
		}

		prevChar = byte;
	}

	if (allNode) return TRUE;

	return FALSE;
}

/*
* writePath.assign("C:\\Users\\Administrator\\Desktop\\");
	writePath.append(name);
	writePath.append(".txt");

	if (!forWrite.is_open())
	{
		forWrite.open(writePath, ios::out | ios::ate);
		if (!forWrite.is_open()) return FALSE;
	}

	forWrite << content;

	forWrite.close();
*/

inline BOOL fApXml::save2Txt(const string& filename)
{
	xml->writePath.assign("C:\\Users\\Administrator\\Desktop\\");
	xml->writePath.append(filename);
	xml->writePath.append(".txt");

	if (!xml->forWrite.is_open())
	{
		xml->forWrite.open(xml->writePath, ios::out | ios::ate);
		if (!xml->forWrite.is_open()) return FALSE;
	}

	Queuenode* head = allNode->getParentQueue()->getHead();
	Attr* attr;

	while (head)
	{
		xml->forWrite << "节点名: ";
		xml->forWrite << xml->utf2gbk(head->node->nodeName);
		xml->forWrite << endl;
		xml->forWrite << "属性:\n";

		attr = head->node->nodeAttr->getHead();
		while (attr)
		{
			xml->forWrite << attr->attrName;
			xml->forWrite << " : ";
			xml->forWrite << attr->attrText;
			xml->forWrite << endl;
			attr = attr->next;
		}
		xml->forWrite << endl;
		xml->forWrite << "节点内容: ";
		xml->forWrite << head->node->nodeText;
		xml->forWrite << endl;

		xmlNode* tempSib = head->node->firstchild;
		while (tempSib && tempSib->firstchild == NULL)
		{
			xml->forWrite << "节点名: ";
			xml->forWrite << tempSib->nodeName;
			xml->forWrite << endl;
			xml->forWrite << "属性:\n";

			attr = tempSib->nodeAttr->getHead();
			while (attr)
			{
				xml->forWrite << attr->attrName;
				xml->forWrite << " : ";
				xml->forWrite << attr->attrText;
				attr = attr->next;
			}
			xml->forWrite << endl;
			xml->forWrite << "节点内容: ";
			xml->forWrite << tempSib->nodeText;
			xml->forWrite << endl;

			tempSib = tempSib->rightsibling;
		}

		head = head->next;
	}

	return TRUE;
}
