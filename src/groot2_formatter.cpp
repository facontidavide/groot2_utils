#include "cxxopts/cxxopts.hpp"
#include "xml_formatter.hpp"
#include <iostream>
#include <fstream>

int main(int argc, char** argv)
{
  cxxopts::Options options("groot2_formatter", "Parse an XML file and create a new one "
                                               "using the Groot2 rules");
  // clang-format off
  options.add_options()
    ("i,input", "Input file", cxxopts::value<std::string>())
    ("o,output", "Output file", cxxopts::value<std::string>())
    ("check", "Check if the XML is correctly formatted")
    ("inplace", "Change the file inplace")
    ("h,help", "Print this message");
  // clang-format on

  auto result = options.parse(argc, argv);
  if(result.count("help"))
  {
    std::cout << options.help() << std::endl;
    return 0;
  }

  const std::string file_in = result["input"].as<std::string>();
  std::cout << "Input file: " << file_in << std::endl;

  const bool check = result.count("check");
  const bool inplace = result.count("inplace");
  const bool has_fileout = result.count("output");

  if(!inplace && !check && !has_fileout)
  {
    std::cout << "Use either the argument --output, --check or --inplace " << std::endl;
    return -1;
  }
  const auto file_out = has_fileout ? result["output"].as<std::string>() : std::string();

  std::ifstream stream_in(file_in);
  std::stringstream buffer;
  buffer << stream_in.rdbuf();
  const std::string xml_in = buffer.str();

  tinyxml2::XMLDocument doc;
  auto parsed = doc.Parse(xml_in.data(), xml_in.size());

  if(parsed != tinyxml2::XML_SUCCESS)
  {
    doc.PrintError();
    return 1;
  }

  groot2::CustomXmlPrinter printer;
  doc.Print(&printer);

  const std::string xml_out(printer.CStr(), printer.CStrSize() - 1);

  const bool is_same = (xml_in == xml_out);

  if(is_same)
  {
    std::cout << "Input and output XMLs are the SAME\n";
  }
  else
  {
    std::cout << "Input and output XMLs are the NOT the same\n";
  }

  if(!is_same && (inplace || (has_fileout && file_in == file_out)))
  {
    std::ofstream ofstream(file_in, std::ofstream::out);
    std::cout << "Writing new XML into file: " << file_in << std::endl;
    ofstream << xml_out;
  }

  if(has_fileout && file_in != file_out)
  {
    std::ofstream ofstream(file_out, std::ofstream::out);
    std::cout << "Writing new XML into file: " << file_out << std::endl;
    ofstream << xml_out;
  }

  if(check && !is_same)
  {
    return 1;
  }

  return 0;
}
