#include "xml_formatter.hpp"
#include <string>
#include <vector>

namespace groot2
{
const char* LF = "&#10;";
const int LF_Size = 5;
const char* NewLine = "\n";
const int NewLine_Size = 1;

void BaseXmlPrinter::Write(const char* data, size_t size)
{
  std::string stringData(data, size);
  size_t index = 0;
  while((index = stringData.find(NewLine, index)) != std::string::npos)
  {
    stringData.replace(index, NewLine_Size, LF);
  }
  tinyxml2::XMLPrinter::Write(stringData.c_str(), stringData.size());
}

void CustomXmlPrinter::PrintSpace(int depth)
{
  for(int i = 0; i < depth; i++)
  {
    tinyxml2::XMLPrinter::Write("  ");
  }
}

bool CustomXmlPrinter::VisitExit(const tinyxml2::XMLElement& element)
{
  auto ret = tinyxml2::XMLPrinter::VisitExit(element);

  const tinyxml2::XMLElement* parentElem = nullptr;
  if(element.Parent())
  {
    parentElem = element.Parent()->ToElement();
  }

  const auto root_tag = std::string("root");
  if(parentElem && root_tag == parentElem->Name())
  {
    Putc('\n');
  }
  return ret;
}

bool CustomXmlPrinter::VisitEnter(const tinyxml2::XMLElement& element,
                                  const tinyxml2::XMLAttribute* attribute)
{
  OpenElement(element.Name(), false);

  if(attribute)
  {
    std::vector<const tinyxml2::XMLAttribute*> attributes;
    while(attribute)
    {
      attributes.push_back(attribute);
      attribute = attribute->Next();
    }
    size_t const attr_count = attributes.size();

    int depth = 0;

    if(attr_count > 1)
    {
      int index = CStrSize();
      while(index > 0)
      {
        index--;
        char ch = CStr()[index];
        if(ch == '\n')
        {
          break;
        }
        depth++;
      }
    }

    for(size_t a = 0; a < attr_count; a++)
    {
      attribute = attributes[a];

      PushAttribute(attribute->Name(), attribute->Value());
      if(a + 1 < attr_count)
      {
        Putc('\n');
        for(int i = 1; i < depth; ++i)
        {
          Putc(' ');
        }
      }
    }
  }
  return true;
}

}  // namespace groot2
