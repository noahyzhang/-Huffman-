#include"FileCompressHuff.hpp"


int main()
{
	const char* sou_file_path = "E://vs 项目/Huffman压缩/data/1.pdf";
	const char* com_file_path = "E://vs 项目/Huffman压缩/data/1.zyhzp";
	const char* common = "E://vs 项目/Huffman压缩/data/";
	FileHuffmanCompress fhc;
	fhc.CompressFile(sou_file_path, com_file_path);
	fhc.UnCompressFile(com_file_path, common);
	//fhc.UNCompressFile(com_file_path);
	return 0;
}