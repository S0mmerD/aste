#pragma once
#include "precice/SolverInterface.hpp"
#include "utils/prettyprint.hpp"
#include "utils/EventTimings.hpp"
#include "mpiUtils.hpp"
#include "common.hpp"
enum class Participant
{
    A,
    B
};

std::string toString(Participant p){
    return p == Participant::A? "Participant A" : "Participant B";
}

class Precice
{
    int _meshID;
    int _dataID;
    public:
        Precice(Participant participant, std::shared_ptr<MPIEnv> mpi, std::string runName, std::string config)
        {
            precice::SolverInterface interface(toString(participant), mpi->rank(), mpi->size());
            interface.configure(config);
            precice::utils::EventRegistry::instance().runName = runName;
            _meshID = interface.getMeshID(participant == Participant::A? "MeshA" : "MeshB" );
            _dataID = interface.getDataID("Data", _meshID);
        }
};

Precice makePreciceFromOptions(std::shared_ptr<MPIEnv> env, OptionMap options)
{
    using std::string;
    auto part = options["participant"].as<string>() == "A" ? Participant::A : Participant::B;
    auto runName = options["runName"].as<std::string>();
    auto preciceConfig = options["precice-config"].as<string>();
    return Precice(part, env, runName, preciceConfig);
}
