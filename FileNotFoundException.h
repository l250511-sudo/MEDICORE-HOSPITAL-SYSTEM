#pragma once
#include "HospitalException.h"

class FileNotFoundException : public HospitalException {
public:
    FileNotFoundException(); // Just the semicolon!
};