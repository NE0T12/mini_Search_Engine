# mini Web Search Engine

## 离线部分
	
	生成网页库和偏移库以及倒排索引表（TF-ITF算法）

## 线上部分

	提供搜索服务。（采用余弦相似度算法）

## 版本

### version 0.0  date--20180613
	
	mkdir

### version 0.1  date--20180615
	
	生成ripepage.lib、offset.lib

### version 0.4  date--20180620
	
	离线部分基本完成(相关类的代码实现，调试运行，bug排查修复)
	生成newripepage.lib(去重后的网页库)、newoffset.lib(去重后的偏移库)、invertIndex.lib(倒排索引表)

### version 0.6  date--20180625
	
	初步完成

### version 0.8  date--20180625
	
	将英文单词大写变小写（服务端数据以及客户端搜索词）；添加redis缓存
