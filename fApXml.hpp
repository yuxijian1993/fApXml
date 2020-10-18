#pragma once

#include "fApFile.hpp"

using namespace std;

enum CurState
{
	READ_NORMAL,			//������ȡ
	USELESS_BYTE,			//���õ��ַ�
	SAVE_NAME,				//����ڵ���
	NAME_OVER,
	//SAVE_ATTRNAME,			//����������
	//ATTRNAME_OVER,			//
	//SAVE_ATTR,				//������������
	//ATTR_OVER,
	SAVE_TEXT,				//����ڵ�����
	TEXT_OVER
};

//�ڵ㵥������
class Attr
{
public:
	Attr() :attrName(""), attrText(""), next(NULL) {};
	void initAttr();

	//�ڵ�������
	string attrName;
	//�ڵ�����ֵ
	string attrText;
	//��һ�����Ե�ָ��
	Attr* next;
};

//�ڵ���������
class AttrList
{
public:
	//��ʼ��
	AttrList() :head(NULL), latest(NULL), work(NULL), attrNum(0) {};
	//��ȡͷ���
	Attr* getHead();
	//������Խڵ�
	BOOL addAttrNode(Attr* an);
	//ͨ��������������������
	Attr* searchAttr(const string& attrname);
	//ͨ��������ɾ�����Խڵ�
	BOOL deleteAttr(const string& attrname);
	//�����������
	BOOL clearAttr();
	//	��ʾor���
	BOOL showAllattr();

private:
	//��һ������
	Attr* head;
	//���һ������
	Attr* latest;
	//����ָ��
	Attr* work;
	//��������ڵ����
	INT attrNum;
};

//���ڵ�
class xmlNode
{
public:
	xmlNode();


	//�ڵ���
	string nodeName;
	//�ڵ�����
	string nodeText;
	//�ڵ���������
	AttrList* nodeAttr;
	//��һ���ӽڵ�
	xmlNode* firstchild;
	//���ֵ�
	xmlNode* rightsibling;
	//�ýڵ��Ƿ��Ѿ����ڹ��ڵ������־
	BOOL isClosed;
	//�ڵ���,��������ڵ��޷����ҵ�����
	INT index;
};

//���нڵ�
class Queuenode
{
public:
	Queuenode() :node(NULL), next(NULL) {};

	xmlNode* node;
	Queuenode* next;
};

//˫�׶���
class Queue
{
public:
	Queue() :head(NULL), latest(NULL), work(NULL) {};
	//β����
	BOOL QueueInsert(xmlNode* cur);
	//ͷɾ��
	BOOL QueueDelelte();
	//���
	BOOL clearQueue();
	//����
	BOOL isInQueue(xmlNode* node);
	//��ʾor���
	BOOL showAll();
	//��ȡ����ͷ
	Queuenode* getHead();


	Queuenode* head;
	Queuenode* latest;
	Queuenode* work;
};

//��
//�洢���е��ֵܽڵ�
class TreeNode
{
public:
	TreeNode();
	//����ֵ�
	BOOL addSibling(const string& name, const string& text = "");
	//����ӽڵ�
	BOOL addChild(const string& name, const string& text = "");
	//������ṹ�ڵ�--�ڲ��ж�����ֵܽڵ���ӽڵ�
	BOOL addTreeNode(const string& name, const string& text = "");
	//���������ڵ�����
	xmlNode* searchTree(const string& name, const INT& i);
	//�ж��Ƿ��ǽڵ������־
	BOOL isNodeOver(const string& name);
	//ͨ���ڵ������־���ýڵ����״̬
	BOOL setNodeOver(const string& name);
	//��֮ǰ�����Ľڵ��������
	BOOL setLastNodeAttribute(Attr* attr);
	//��֮ǰ�����Ľڵ���ӽڵ�����
	BOOL setLastNodeText(const string& text);
	//��ȡ�������ӽڵ�Ľڵ�
	Queue* getParentQueue();
	



private:
	xmlNode* root;
	xmlNode* latest;
	xmlNode* work;

	//�������е�parent�ڵ�
	Queue* parent;
	//�ֵܸ���
	INT sibNum;
};

class fApXml
{
public:
	//��ʼ��
	fApXml();
	//�����ַ��ж�
	BOOL filterChar(const char& c);
	//�ַ������
	//��ִ��ַ�����ʼ��over֮ǰ�������ַ�
	//����ɾ��ԭ�ַ����в�ֳ���������,������ַ�Ҳ���ԭ�ַ�����ɾ��
	string splitWord(const char& end, string& res);
	//��ִ�һ���ַ�����һ���ַ�֮����ַ���
	//����ɾ��ԭ�ַ����в�ֳ���������,������ַ�Ҳ���ԭ�ַ�����ɾ��
	string splitWord(const char& begin, const char& end, string& res);
	//�ж��Ƿ���xmlע��
	BOOL isXMLnote(const string& res);


	//xml����ת���ṹ
	BOOL Xml2Tree();
	//save
	BOOL save2Txt(const string& filename = "out");


//private:
	fApFile* xml;
	TreeNode* allNode;
};

//		class Attr ����ʵ��

void Attr::initAttr()
{
	attrName = "";
	attrText = "";
	next = NULL;
}

//		class AttrList ����ʵ��

//��ȡͷ���
inline Attr* AttrList::getHead()
{
	return head;
}

//������Խڵ�
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

//ͨ��������������������
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

//ͨ��������ɾ�����Խڵ�
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

//�����������
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

//		class xmlNode ����ʵ��

xmlNode::xmlNode()
{
	nodeName = "";
	nodeText = "";
	nodeAttr = new AttrList();
	firstchild = NULL;
	rightsibling = NULL;
	isClosed = FALSE;
}

//		class Queue ����ʵ��

//β����
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

//ͷɾ��
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

//���
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
		cout << "�ڵ���: " << work->node->nodeName << "\n";
		cout << "����: ";
		work->node->nodeAttr->showAllattr();
		cout << endl;
		cout << "�ڵ�����: " << work->node->nodeText;
		cout << endl;

		work = work->next;
	}

	return TRUE;
}

inline Queuenode* Queue::getHead()
{
	return head;
}

//class TrreNode ����ʵ��

TreeNode::TreeNode()
{
	root = NULL;
	latest = NULL;
	work = NULL;
	parent = new Queue();
	sibNum = 0;
}

//����ֵ�
BOOL TreeNode::addSibling(const string& name, const string& text)
{
	//���ڵ㲻����(������)������ֱ�ӵ���ֵܽڵ�
	//�����ֻ�и��ڵ�,��ֻ���ڸ��ڵ�������ӽڵ�
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

//�����ӽڵ�
BOOL TreeNode::addChild(const string& name, const string& text)
{
	//û�и��ڵ�,����������ӽڵ�
	if (root == NULL) return NULL;
	//��ӵ�һ���ֽڵ�˵��˫�׶�����û�е�ǰ�ڵ�
	//�ѵ�ǰ�ڵ��ӽ�˫�׶���
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

//		class fApXml ����ʵ��

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
	//��ʱ�����ȡ���ַ���
	string word = "";
	//��ǰ��ȡ״̬��ʼ��δ������ȡ״̬
	INT state = READ_NORMAL;
	//��ʱ��������
	Attr* tempAttr = new Attr();
	//��ǰ��ȡ���ַ�
	CHAR byte = 0;
	//����֮ǰ��ȡ���ַ�
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
		//״̬Ϊ��������ʱ,�Ѷ�ȡ���ַ����浽�ַ�����
		case SAVE_NAME:
		//case SAVE_ATTRNAME:
		//case SAVE_ATTR:
		case SAVE_TEXT:
			word.append(1, byte);
			break;
		//�ڵ����������
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

			//����ڵ����д��ڡ� ��(�ո�),��˵���˽ڵ��������
			if (word.find(' ') != word.npos)
			{
				//�������

				//��ֳ��ڵ���
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
		//�������������
		/*case ATTRNAME_OVER:
			tempAttr->attrName.assign(word);
			word = "";
			state = READ_NORMAL;
			break;
		//�������ݱ������
		case ATTR_OVER:
			tempAttr->attrText.assign(word);
			word = "";

			allNode->setLastNodeAttribute(tempAttr);
			tempAttr->initAttr();

			state = READ_NORMAL;
			break;*/
		//�ڵ����ݱ������
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
		xml->forWrite << "�ڵ���: ";
		xml->forWrite << xml->utf2gbk(head->node->nodeName);
		xml->forWrite << endl;
		xml->forWrite << "����:\n";

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
		xml->forWrite << "�ڵ�����: ";
		xml->forWrite << head->node->nodeText;
		xml->forWrite << endl;

		xmlNode* tempSib = head->node->firstchild;
		while (tempSib && tempSib->firstchild == NULL)
		{
			xml->forWrite << "�ڵ���: ";
			xml->forWrite << tempSib->nodeName;
			xml->forWrite << endl;
			xml->forWrite << "����:\n";

			attr = tempSib->nodeAttr->getHead();
			while (attr)
			{
				xml->forWrite << attr->attrName;
				xml->forWrite << " : ";
				xml->forWrite << attr->attrText;
				attr = attr->next;
			}
			xml->forWrite << endl;
			xml->forWrite << "�ڵ�����: ";
			xml->forWrite << tempSib->nodeText;
			xml->forWrite << endl;

			tempSib = tempSib->rightsibling;
		}

		head = head->next;
	}

	return TRUE;
}
