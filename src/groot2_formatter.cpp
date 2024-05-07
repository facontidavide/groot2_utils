#include "cxxopts/cxxopts.hpp"
#include "xml_formatter.hpp"
#include <iostream>
#include <fstream>

int main(int argc, char** argv)
{
  cxxopts::Options options("groot2_formatter", "Parse an XML file and create a new one "
                                               "using the Groot2 rules");

  options.add_options()("i,input", "Input file", cxxopts::value<std::string>())(
      "o,output", "Output file", cxxopts::value<std::string>())
      // ("m,models", "(Optional) BT models file", cxxopts::value<std::string>())
      ("h,help", "Print this message");

  auto result = options.parse(argc, argv);
  if(result.count("help"))
  {
    std::cout << options.help() << std::endl;
    exit(0);
  }

  std::string file_in = result["input"].as<std::string>();
  std::cout << "Input file: " << file_in << std::endl;

  tinyxml2::XMLDocument doc;
  auto parsed = doc.LoadFile(file_in.c_str());

  if(parsed != tinyxml2::XML_SUCCESS)
  {
    doc.PrintError();
    return 1;
  }

  groot2::CustomXmlPrinter printer;
  doc.Print(&printer);

  std::string out(printer.CStr(), printer.CStrSize() - 1);

  if(result.count("output"))
  {
    std::string file_out = result["output"].as<std::string>();
    std::cout << "Output file: " << file_out << std::endl;
    std::ofstream ofstream(file_out, std::ofstream::out);
    ofstream << out;
  }
  else
  {
    std::cout << "\n Output:\n" << out << std::endl;
  }

  return 0;
}
