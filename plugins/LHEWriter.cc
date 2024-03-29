#include <algorithm>
#include <iostream>
#include <iterator>
#include <fstream>
#include <string>
#include <memory>

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"

using namespace lhef;

class LHEWriter : public edm::EDAnalyzer {
    public:
	explicit LHEWriter(const edm::ParameterSet &params);
	virtual ~LHEWriter();

    protected:
	virtual void beginRun(const edm::Run &run, const edm::EventSetup &es);
	virtual void endRun(const edm::Run &run, const edm::EventSetup &es);
	virtual void analyze(const edm::Event &event, const edm::EventSetup &es);

    private:
	std::ofstream	file;
        std::string filename;
};

LHEWriter::LHEWriter(const edm::ParameterSet &params):
filename(params.getParameter<std::string>("ouputFile"))
{
}

LHEWriter::~LHEWriter()
{
}

void LHEWriter::beginRun(const edm::Run &run, const edm::EventSetup &es)
{
	edm::Handle<LHERunInfoProduct> product;
	run.getByLabel("source", product);

	file.open(filename.c_str(), std::fstream::out | std::fstream::trunc);
	std::copy(product->begin(), product->end(),
	          std::ostream_iterator<std::string>(file));
}

void LHEWriter::endRun(const edm::Run &run, const edm::EventSetup &es)
{
	file << LHERunInfoProduct::endOfFile();
	file.close();
}

void LHEWriter::analyze(const edm::Event &event, const edm::EventSetup &es)
{
	edm::Handle<LHEEventProduct> product;
	event.getByLabel("source", product);

	std::copy(product->begin(), product->end(),
	          std::ostream_iterator<std::string>(file));
}

DEFINE_FWK_MODULE(LHEWriter);
