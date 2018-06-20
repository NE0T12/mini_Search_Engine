 ///
 /// @file    FileProcessor.cc
 /// @author  ne0t12
 /// @date    2018-06-15 03:21:46
 ///

#include "../include/FileProcessor.h"

using namespace MSE;
using namespace tinyxml2;

FileProcessor::FileProcessor()
{
	_rssItem_vec.reserve(300);
}

void FileProcessor::process(const string & filepath)
{
	XMLDocument doc;
	doc.LoadFile(filepath.c_str());

	//"链式编程"
	XMLElement * proot = doc.FirstChildElement("rss");                                                                                                                   
	if(proot)
	{
		XMLElement * pchannel = proot->FirstChildElement("channel");
		if(pchannel)
		{
			XMLElement * pitem = pchannel->FirstChildElement("item");
			while(pitem)
			{
				RssItem rssItem;

				XMLElement * ptitle = pitem->FirstChildElement("title");
				XMLElement * plink = pitem->FirstChildElement("link");
				XMLElement * pdescription = pitem->FirstChildElement("description");
				XMLElement * pcontent = pitem->FirstChildElement("content:encoded");
			
				if(ptitle && ptitle->GetText()) //标题
					rssItem.title = ptitle->GetText();
				else
					goto NEXT;
				
				if(plink && plink->GetText()) //链接url
					rssItem.link = string(plink->GetText());
				else
					goto NEXT;
				
				if(pcontent && pcontent->GetText()) //文本内容
					rssItem.content = string(pcontent->GetText());
				else if(pdescription && pdescription->GetText())
					rssItem.content = string(pdescription->GetText());
				else
					goto NEXT;
				
				if(rssItem.content.size() < 100) //文章内容过短，不要
					goto NEXT;

				_rssItem_vec.push_back(rssItem);

				NEXT:
				pitem = pitem->NextSiblingElement();
			}
		}
	}
}

vector<RssItem> & FileProcessor::get_doc()
{
	return _rssItem_vec;
}
