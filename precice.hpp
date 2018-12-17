#pragma once
#include <memory>

#include "precice/SolverInterface.hpp"
#include "utils/prettyprint.hpp"
#include "utils/EventTimings.hpp"
#include "mpiUtils.hpp"
#include "common.hpp"
#include "mesh/mesh.hpp"
enum class Participant
{
    A,
    B
};

std::string toString(Participant p){
    return p == Participant::A? "A" : "B";
}

    template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

class Precice
{
    int _meshID;
    int _dataID;
    int _count;
    std::vector<int> _vertexIDs;
    Participant _participant;
    std::shared_ptr<Mesh> _mesh;
    std::vector<double> _data;
    std::vector<Vertex> _ownVertices;
    std::string _tag;
    std::unique_ptr<precice::SolverInterface> _interface;
    public:
    Precice(Participant participant, std::shared_ptr<MPIEnv> mpi, std::string runName, std::string config, std::shared_ptr<Mesh> mesh, std::string dataTag) : _participant{participant}, _mesh{mesh}, _tag{dataTag}
    {
        _interface = make_unique<precice::SolverInterface>(toString(participant), mpi->rank(), mpi->size());
        _interface->configure(config);
        precice::utils::EventRegistry::instance().runName = runName;
        _meshID = _interface->getMeshID(participant == Participant::A? "MeshA" : "MeshB" );
        _dataID = _interface->getDataID("Data", _meshID);
        std::cout << "preparing vertices\n";
        _vertexIDs.reserve(mesh->pointCount());
        auto secondIt = mesh->allValues(_tag).begin();
        auto rank = mpi->rank();
        for (const auto& vert : mesh->allPoints()){
            if (*secondIt == rank){
                _vertexIDs.push_back(_interface->setMeshVertex(_meshID, vert.data()));
                _ownVertices.push_back(vert);
            }
            secondIt++;
        }
        _count = _vertexIDs.size();
        _interface->initialize();
        std::cout << "Initialized";
        if (participant == Participant::A)
            _data = std::vector<double>(_count, rank);
        else
            _data = std::vector<double>(_count, 0);
        if (_interface->isActionRequired(precice::constants::actionWriteInitialData())) {
            _interface->writeBlockScalarData(_dataID, _mesh->valueCount(_tag), _vertexIDs.data(), _data.data());
            _interface->fulfilledAction(precice::constants::actionWriteInitialData());
        }
        _interface->initializeData();

    }
    void couple()
    {
        while (_interface->isCouplingOngoing()) {
            if (_participant == Participant::A) {
                _interface->writeBlockScalarData(_dataID, _count, _vertexIDs.data(), _data.data());
            }

            _interface->advance(1);
            if (_participant == Participant::B) {
                _interface->readBlockScalarData(_dataID, _count, _vertexIDs.data(), _data.data()); 
            }
        }
        _interface->finalize();
    }
};

Precice makePreciceFromOptions(std::shared_ptr<MPIEnv> env, OptionMap options, std::shared_ptr<Mesh> mesh)
{
    using std::string;
    auto part = options["participant"].as<string>() == "A" ? Participant::A : Participant::B;
    auto runName = options["runName"].as<std::string>();
    auto preciceConfig = options["precice-config"].as<string>();
    auto tag = options["valueTag"].as<string>();
    return Precice(part, env, runName, preciceConfig, mesh, tag);
}
