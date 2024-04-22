#pragma once

#include "tinyxml2/tinyxml2.h"

namespace groot2
{
class BaseXmlPrinter : public tinyxml2::XMLPrinter
{
public:
  BaseXmlPrinter() : tinyxml2::XMLPrinter(nullptr, false, 0)
  {}

protected:
  void Write(const char* data, size_t size) override;
};

class CustomXmlPrinter : public BaseXmlPrinter
{
public:
  CustomXmlPrinter() : BaseXmlPrinter()
  {}

  void PrintSpace(int depth) override;

  bool VisitExit(const tinyxml2::XMLElement& element) override;

  bool VisitEnter(const tinyxml2::XMLElement& element,
                  const tinyxml2::XMLAttribute* attribute) override;
};

}  // namespace groot2
