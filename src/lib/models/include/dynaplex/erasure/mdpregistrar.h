#pragma once

#include "dynaplex/vargroup.h"        
#include "dynaplex/registry.h"        
#include "makegeneric.h"
#include "mdpadapter.h" 
namespace DynaPlex::Erasure {
    template<typename SpecificMDP>
    class MDPRegistrar {
    public:
        MDPRegistrar(const std::string& modelName, const std::string& model_description = "") {
            DynaPlex::Registry::Register(modelName, model_description, &MDPRegistrar::CreateInstance);
        }

        static DynaPlex::MDP CreateInstance(const VarGroup& vars) {
            return MakeGenericMDP<SpecificMDP>(vars);
        }
    };
}