#include "adif2xml.hpp"
#include "adif2xml_version.hpp"
#include "getopt.h"

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>
#include <xercesc/sax/AttributeList.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

#include "date/date.h"
#include "adifio.hpp"
#include "ediio.hpp"
#include "attributelistimpl.hpp"
#include "SAXPrint.hpp"

using namespace std;
using namespace date;
using namespace ADIFIO;
using namespace EDIIO;
using namespace XERCES_CPP_NAMESPACE;

static void help(const char *name)
{
    cerr << "Usage: " << name << endl
         << "\t-i <filename> ............ Read ADIF file from <filename> (default: STDIN)" << endl
         << "\t-o <filename> ............ Write XMLfile to <filename> (defalt: STDOUT)" << endl
         << "\t-v ....................... Write verbose messages to STDERR" << endl
         << "\t-h ....................... Print help (this message)" << endl;
}

static void headerCB(const ObjectMap_t &tokens, void *userData)
{
    SAXPrintHandlers &ph{*static_cast<SAXPrintHandlers*>(userData)};

    AttributeListImpl empty_list;
    ph.stream() << " ";
    ph.startElement(X("Header"), empty_list);
    ph.stream() << endl;
    for_each(tokens.begin(), tokens.end(), [&](auto &pair) {
        const string &key{pair.first};
        const Object &obj{pair.second};
        const string  str{obj.to_string()};
        AttributeListImpl rg_a;
        rg_a.add("type", Object::getObjectTypeName(obj.type).c_str());
        ph.stream() << "  ";
        ph.startElement(X(key.c_str()), rg_a);
        ph.characters(X(str.c_str()), str.length());
        ph.endElement(X(key.c_str()));
        ph.stream() << endl;
    }); // end for_each //
    ph.stream() << " ";
    ph.endElement(X("Header"));
    ph.stream() << endl;
#if 0
    ostream &oss{*static_cast<ostream*>(userData)};

    oss << "Header:" << endl;
    for_each(tokens.begin(), tokens.end(), [&](auto &pair) {
        const string &key{pair.first};
        const Object &val{pair.second};
        oss << key << " = " << val.to_string() << endl;
    }); // end for_each //
#endif
}

static void recordCB(const ObjectMap_t &tokens, void *userData)
{
    SAXPrintHandlers &ph{*static_cast<SAXPrintHandlers*>(userData)};

    AttributeListImpl empty_list;
    ph.stream() << " ";
    ph.startElement(X("Record"), empty_list);
    ph.stream() << endl;
    for_each(tokens.begin(), tokens.end(), [&](auto &pair) {
        const string &key{pair.first};
        const Object &obj{pair.second};
        const string  str{obj.to_string()};
        AttributeListImpl rg_a;
        rg_a.add("type", Object::getObjectTypeName(obj.type).c_str());
        ph.stream() << "  ";
        ph.startElement(X(key.c_str()), rg_a);
        ph.characters(X(str.c_str()), str.length());
        ph.endElement(X(key.c_str()));
        ph.stream() << endl;
    }); // end for_each //
    ph.stream() << " ";
    ph.endElement(X("Record"));
    ph.stream() << endl;
#if 0
    ostream &oss{*static_cast<ostream*>(userData)};

#define EDI
#ifdef EDI
    write_qso_record(oss, tokens);
#else
    oss << "Record:" << endl;
    for_each(tokens.begin(), tokens.end(), [&](auto &pair) {
        const string &key{pair.first};
        const Object &val{pair.second};
        oss << key << " = " << val.to_string() << endl;
    }); // end for_each //
#endif
#endif
}

int main(int argc, char* argv[]) {
    int option{0}, verbose{0};
    string inputfile{""}, outputfile{""};

    // Get options:
    while ((option = getopt(argc, argv, "hi:o:v")) >= 0) {
        switch (option) {
        case 'h':
            help(argv[0]);
            return EXIT_SUCCESS;
        case 'i':
            if (inputfile.length() > 0) {
                help(argv[0]);
                return EXIT_FAILURE;
            }
            inputfile = optarg;
            break;
        case 'o':
            if (outputfile.length() > 0) {
                help(argv[0]);
                return EXIT_FAILURE;
            }
            outputfile = optarg;
            break;
        case 'v':
            ++verbose;
            break;
        default:
            help(argv[0]);
            return EXIT_FAILURE;
        } // end switch //
    } // end while //

    if (verbose)
        cerr << "This is " APP_NAME << " version " << APP_VERSION << endl
             << "  Copyright (c) " << APP_COPYRIGHT << " see <" << APP_WEBSITE << ">" << endl;

    bool ok{true};

    try {
        istream *input{(inputfile.length() > 0) ? new ifstream(inputfile) : &cin};
        ostream *output{(outputfile.length() > 0) ? new ofstream(outputfile) : &cout};

        if (inputfile.length() > 0) {
            if (!filesystem::exists(inputfile))
                throw runtime_error("File not found: \"" + inputfile + "\"");
            if (input->bad())
                throw runtime_error("Bad inputfile: \"" + inputfile + "\"");
        }
        if (verbose)
            cerr << "Reading ADIF from "
                 << ((inputfile.length() > 0) ? inputfile : "<STDIN>")
                 << endl;

        if (outputfile.length() > 0) {
            if (output->bad())
                throw runtime_error("Bad outputfile: \"" + outputfile + "\"");
        }
        if (verbose)
            cerr << "Writing XML to "
                 << ((outputfile.length() > 0) ? outputfile : "<STDOUT>")
                 << endl;

        XMLPlatformUtils::Initialize();
        ADIFIO::Reader r;
        XMLFormatter::UnRepFlags unrepFlags{XMLFormatter::UnRep_CharRef};
        SAXPrintHandlers printHandlers(*output, "UTF-8", unrepFlags);
        printHandlers.startDocument();
        AttributeListImpl empty_attribute_list;
        printHandlers.startElement(X("ADIF"), empty_attribute_list);
        printHandlers.stream() << endl;
        r.readADI(*input, recordCB, &printHandlers, headerCB, &printHandlers);
        printHandlers.endElement(X("ADIF"));
        printHandlers.stream() << endl;
        printHandlers.endDocument();
    }
    catch (const OutOfMemoryException&)
    {
        cerr << "OutOfMemoryException" << endl;
        ok = false;
    }
    catch (const XMLException& toCatch)
    {
        cerr << "XML error:" << endl
             << StrX(toCatch.getMessage()) << endl;
        ok = false;
    }
    catch (const exception& ex) {
        cerr << "ERROR: " << ex.what() << endl;
        ok = false;
    }
    catch (...) {
        cerr << "ERROR: Unidentified failure!" << endl;
        ok = false;
    }

    XMLPlatformUtils::Terminate();
    return (ok) ? EXIT_SUCCESS : EXIT_FAILURE;
}
