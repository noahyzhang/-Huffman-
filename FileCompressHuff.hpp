#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include<string>
#include<cstring>
#include"HuffmanTree.hpp"
#include<cassert>

typedef unsigned char UCH;
class CharInfo {
public:
	CharInfo(size_t charCount = 0)
		: _charCount(charCount)
	{}

	CharInfo operator+(const CharInfo& info)
	{
		return CharInfo(_charCount + info._charCount);
	}
	bool operator>(const CharInfo& info)
	{
		return _charCount > info._charCount;
	}
	bool operator<(const CharInfo& info)
	{
		return _charCount < info._charCount;
	}
	bool operator!=(const CharInfo& info) const
	{
		return _charCount != info._charCount;
	}
	bool operator==(const CharInfo& info) const
	{
		return _charCount == info._charCount;
	}

	UCH _char;
	size_t _charCount;
	std::string _strCode;
};

class FileHuffmanCompress {

public:
	FileHuffmanCompress()
	{
		_char_info.resize(256);
		for (size_t i = 0; i < 256; ++i)
		{
			_char_info[i]._char = i;
		}
	}
	void CompressFile(const std::string& SourceFilePath,const std::string& CompressFilePath)  //ѹ���ļ�
	{
		std::ifstream sou_file(SourceFilePath.c_str(), std::ifstream::in | std::ifstream::binary);
		sou_file.seekg(0, std::ios::end);
		int length = sou_file.tellg();
		sou_file.seekg(0, std::ios::beg);
		UCH* buf = new UCH[length];
		sou_file.read((char*)buf, length);
		
		for (size_t i = 0; i < length; ++i)
		{
			_char_info[buf[i]]._charCount++;
		}
		HuffmanTree<CharInfo> hf;
		hf.CreateHuffmanTree(_char_info, CharInfo(0));
		GetHuffmanCode(hf.GetRoot());

		std::ofstream com_file(CompressFilePath.c_str(), std::ofstream::out | std::ofstream::binary);
		WriteHeadInfo(com_file, SourceFilePath);

		char ch = 0;
		char bitCount = 0;
		//sou_file.seekg(0, std::ios::beg);
		//sou_file.read(buf, length);
		std::string resCh;
		for (size_t i = 0; i < length; ++i)  //���ֽ���
		{
			std::string& strCode = _char_info[buf[i]]._strCode;
			for (size_t j = 0; j < strCode.size(); ++j)
			{
				ch <<= 1;
				if ('1' == strCode[j])
					ch |= 1;
				bitCount++;
				if (bitCount == 8)
				{
					resCh.push_back(ch);
					bitCount = 0;
				}
			}
		}
		if (bitCount > 0 && bitCount < 8)   //���һ���ֽڿ��ܻ���� bitCount < 8 �����
		{
			ch <<= (8 - bitCount);
			resCh.push_back(ch);
		}
		com_file.write(resCh.c_str(), resCh.length());
		delete[] buf;
		sou_file.close();
		com_file.close();
	}
#if 1
	void UnCompressFile(const std::string& CompressFilePath,const std::string& UnCompressFilePath)
	{
		std::string strPostFix = CompressFilePath.substr(CompressFilePath.rfind('.'));
		if (".zyhzp" != strPostFix)
		{
			std::cout << "compress file faild!" << std::endl;
			return;
		}
		std::ifstream com_file_path;
		com_file_path.open(CompressFilePath.c_str(), std::ifstream::binary | std::ifstream::in);

		//com_file_path.seekg(0, std::ios::end);
		//long length = com_file_path.tellg();
		//std::cout << length << std::endl;  //�����ļ��Ƿ���ȷ������֤������ȷ��

		//if (com_file_path.is_open())
		//{
		//	while (!com_file_path.eof())
		//		std::cout << (char)com_file_path.get();
		//	com_file_path.close();
		//}                          //ͬ������ļ����Ƿ���ȷ��֤���ǶԵ�

		strPostFix.clear();
		std::getline(com_file_path, strPostFix);  //�õ�Դ�ļ��ĺ�׺

		std::string strContent;
		std::getline(com_file_path, strContent);
		size_t lineCount = atoi(strContent.c_str());

		for (size_t i = 0; i < lineCount; ++i)
		{
			strContent.clear();
			std::getline(com_file_path, strContent);
			if (strContent.empty())
			{
				strContent += '\n';
				std::getline(com_file_path, strContent);
			}
			_char_info[(UCH)strContent[0]]._charCount = atoi(strContent.c_str() + 2); //�õ�һ����ĸ�ĸ�������2����Ϊ��������ĸ֮���2��λ�ã�
		}

		//����huffman��
		HuffmanTree<CharInfo> hf;
		hf.CreateHuffmanTree(_char_info, CharInfo(0));

		std::string strUnComFile;
		strUnComFile += UnCompressFilePath;
		strUnComFile += "UnCompressFile";
		strUnComFile += strPostFix;


		std::ofstream uncom_file(strUnComFile.c_str(), std::ofstream::binary);
		HuffmanTreeNode<CharInfo>* pCur = hf.GetRoot();

		long curPos = com_file_path.tellg();
		com_file_path.seekg(0, std::ios::end);
		long LastPos = com_file_path.tellg();
		long length = LastPos - curPos;
		com_file_path.seekg(curPos);

		char pos = 7;
		size_t fileSize = pCur->_weight._charCount;

		//char buf[1024] = { 0 };
		char* buf = new char[length];
		std::string resStr;


		com_file_path.read(buf, length);
		for (size_t i = 0; i < length; ++i)  //���ǧ������strlen��û��null.....��鰥ѽ����Ұ���ʹ��new�����Ŀռ�����ܡ���ø�����ʼ��ȫ0.Ҫ��Ȼ�ж���������
		{
			pos = 7;
			for (size_t j = 0; j < 8; ++j)
			{
				if (buf[i] & (1 << pos))
					pCur = pCur->_pRight;
				else
					pCur = pCur->_pLeft;
				if (pCur->_pLeft == nullptr && pCur->_pRight == nullptr)
				{
					resStr.push_back(pCur->_weight._char);
					pCur = hf.GetRoot();
					fileSize--;
					if (0 == fileSize)
						break;
				}
				pos--;
			}
		}
	/*	if (com_file_path.gcount() == 0)
			break;*/
			//if (com_file_path.eof())    //���ܻ���ǰ����FF����ô�ͺ����ܡ�
			//	break;
		
		uncom_file.write(resStr.c_str(), resStr.length());
		delete[] buf;
		com_file_path.close();
		uncom_file.close();
	}
#else
	void UNCompressFile(const std::string& strFilePath)
	{
		//���ѹ���ļ��ĺ�׺��ʽ
		std::string strpostFix = strFilePath.substr(strFilePath.rfind('.'));
		if (".zyhzp" != strpostFix)
		{
			std::cout << "ѹ���ļ��ĸ�ʽ������" << std::endl;
			return;
		}
		//��ȡ��ѹ������Ϣ
		FILE* fIn = fopen(strFilePath.c_str(), "rb");
		if (nullptr == fIn)
		{
			std::cout << "ѹ���ļ���ʧ��" << std::endl;
			return;
		}
		//��ȡԴ�ļ��ĺ�׺
		strpostFix = "";
		GetLine(fIn, strpostFix);

		//������
		std::string strContent;
		GetLine(fIn, strContent);
		size_t lineCount = atoi(strContent.c_str());

		//�ַ���Ϣ
		for (size_t i = 0; i < lineCount; ++i)
		{
			strContent = "";
			GetLine(fIn, strContent);
			if (strContent.empty())
			{
				strContent += '\n';
				GetLine(fIn, strContent);
			}
			_char_info[(UCH)strContent[0]]._charCount = atoi(strContent.c_str() + 2);
		}

		//����huffman��
		HuffmanTree<CharInfo> ht;
		ht.CreateHuffmanTree(_char_info, CharInfo(0));

		//��ѹ��
		std::string strUNComFile = "2";
		strUNComFile += strpostFix;
		FILE* fOut = fopen(strUNComFile.c_str(), "wb");
		assert(fOut);
		char* pReadBuff = new char[1024];
		HuffmanTreeNode<CharInfo>* pCur = ht.GetRoot();
		char pos = 7;
		size_t fileSize = pCur->_weight._charCount;
		while (true)
		{
			size_t rdSize = fread(pReadBuff, 1, 1024, fIn);
			if (0 == rdSize)
				break;
			for (size_t i = 0; i < rdSize; ++i)
			{
				pos = 7;
				for (size_t j = 0; j < 8; ++j)
				{
					if (pReadBuff[i] & (1 << pos))
						pCur = pCur->_pRight;
					else
						pCur = pCur->_pLeft;
					if (pCur->_pLeft == nullptr && pCur->_pRight == nullptr)
					{
						fputc(pCur->_weight._char, fOut);
						pCur = ht.GetRoot();
						fileSize--;
						if (0 == fileSize)
							break;
					}
					pos--;
				}
			}
		}
		delete[] pReadBuff;
		fclose(fIn);
		fclose(fOut);
	}
#endif
private:
	void GetHuffmanCode(HuffmanTreeNode<CharInfo>* pRoot) //����huffman����ȡÿ���ַ��ı���
	{
		if (pRoot == nullptr)
			return;
		GetHuffmanCode(pRoot->_pLeft);
		GetHuffmanCode(pRoot->_pRight);
		if (pRoot->_pLeft == nullptr && pRoot->_pRight == nullptr)
		{
			HuffmanTreeNode<CharInfo>* pCur = pRoot;
			HuffmanTreeNode<CharInfo>* pParent = pRoot->_pParent;
			std::string& strCode = _char_info[pCur->_weight._char]._strCode;  //���һ��Ҫ�������ã�����ͻ�Ĳ���
			while (pParent)
			{
				if (pParent->_pLeft == pCur)
					strCode += '0';
				else
					strCode += '1';
				pCur = pParent;
				pParent = pCur->_pParent;
			}
			std::reverse(strCode.begin(), strCode.end());
		}
	}
	void WriteHeadInfo(std::ofstream& com_file, const std::string& SourceFilePath)
	{
		std::string strHeadInfo;
		strHeadInfo = SourceFilePath.substr(SourceFilePath.rfind('.'));  //��һ�д����Դ�ļ��ĺ�׺
		strHeadInfo += '\n';

		std::string strCharInfo;
		char szCount[33];
		size_t lineCount = 0;
		for (size_t i = 0; i < 256; ++i) //��ascii����С��������
		{
			if (_char_info[i]._charCount)
			{
				strCharInfo += _char_info[i]._char;  //������ĸ
				strCharInfo += ',';
				_itoa(_char_info[i]._charCount, szCount, 10);
				strCharInfo += szCount; //������ĸ����
				strCharInfo += '\n';
				lineCount++;
			}
		}
		_itoa(lineCount, szCount, 10);
		strHeadInfo += szCount; //��������
		strHeadInfo += '\n';

		strHeadInfo += strCharInfo;
		com_file.write(strHeadInfo.c_str(), strHeadInfo.length());  //д���ļ���
	}

	void GetLine(FILE* fIn, std::string& strContent)
	{
		while (!feof(fIn))
		{
			char ch = fgetc(fIn);
			if ('\n' == ch)
				return;
			strContent += ch;
		}
	}
private:
	std::vector<CharInfo> _char_info;
};