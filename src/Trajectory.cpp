/* Chemfiles, an efficient IO library for chemistry file formats
* Copyright (C) 2015 Guillaume Fraux
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/
*/

#include "chemfiles/Trajectory.hpp"
#include "chemfiles/TrajectoryFactory.hpp"
#include "chemfiles/Logger.hpp"
#include "chemfiles/files/BasicFile.hpp"

using namespace chemfiles;
using std::string;

//! Get the extension part of a filename. Filename is assumed to be valid here.
static string extension(const string& filename) {
    auto idx = filename.rfind('.');

    if(idx != std::string::npos) {
        return filename.substr(idx);
    } else {
        return "";
    }
}

Trajectory::Trajectory(const string& filename, const string& mode, const string& format)
: step_(0), nsteps_(0), format_(nullptr), file_(nullptr), topology_(), use_custom_topology_(false), cell_(), use_custom_cell_(false) {
    trajectory_builder_t builder;
    if (format == ""){
        // try to guess the format by extension
        auto ext = extension(filename);
        builder = TrajectoryFactory::get().by_extension(ext);
    }
    else {
        builder = TrajectoryFactory::get().format(format);
    }

    file_ = builder.file_creator(filename, mode);
    format_ = builder.format_creator(*file_);

    if (mode == "r" || mode == "a")
        nsteps_ = format_->nsteps();
}

Trajectory::~Trajectory(){}

Trajectory& Trajectory::operator>>(Frame& frame){
    frame = read();
    return *this;
}

Frame Trajectory::read(){
    if (step_ >= nsteps_) {
        throw FileError("Can not read file \"" + file_->filename() + "\" past end.");
    }
    if (!(file_->mode() == "r" || file_->mode() == "a")) {
        throw FileError("File \"" + file_->filename() + "\" was not openened in read or append mode.");
    }

    Frame frame;
    format_->read(frame);
    step_++;

    // Set the frame topology if needed
    if (use_custom_topology_)
        frame.topology(topology_);

    // Set the frame unit cell if needed
    if (use_custom_cell_)
        frame.cell(cell_);

    return frame;
}

Frame Trajectory::read_step(const size_t step){
    if (step >= nsteps_) {
        throw FileError(
            "Can not read file \"" + file_->filename() + "\" at step " +
            std::to_string(step) + ". Max step is " + std::to_string(nsteps_) + "."
        );
    }
    if (!(file_->mode() == "r" || file_->mode() == "a")) {
        throw FileError("File \"" + file_->filename() + "\" was not openened in read or append mode.");
    }

    Frame frame;
    step_ = step;
    format_->read_step(step_, frame);

    // Set the frame topology if needed
    if (use_custom_topology_)
        frame.topology(topology_);

    // Set the frame unit cell if needed
    if (use_custom_cell_)
        frame.cell(cell_);

    return frame;
}

Trajectory& Trajectory::operator<<(const Frame& frame){
    write(frame);
    return *this;
}

void Trajectory::write(const Frame& input_frame){
    if (!(file_->mode() == "w" || file_->mode() == "a")) {
        throw FileError("File \"" + file_->filename() + "\" was not openened in write or append mode.");
    }

    // Maybe that is not the better way to do this, performance-wise. I'll have
    // to benchmark this part.
    Frame frame = input_frame;
    if (use_custom_topology_)
        frame.topology(topology_);
    if (use_custom_cell_)
        frame.cell(cell_);

    format_->write(frame);
    step_++;
    nsteps_++;
}

void Trajectory::topology(const Topology& top){
    use_custom_topology_ = true;
    topology_ = top;
}

void Trajectory::topology(const std::string& filename) {
    Trajectory topolgy_file(filename, "r");
    assert(topolgy_file.nsteps() > 0);

    auto frame = topolgy_file.read_step(0);
    Trajectory::topology(frame.topology());
}

void Trajectory::cell(const UnitCell& new_cell){
    use_custom_cell_ = true;
    cell_ = new_cell;
}

void Trajectory::sync() {
    file_->sync();
}

bool Trajectory::done() const {
    return step_ >= nsteps_;
}
