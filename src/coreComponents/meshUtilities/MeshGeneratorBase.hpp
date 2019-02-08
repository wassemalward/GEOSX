/*
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Copyright (c) 2019, Lawrence Livermore National Security, LLC.
 *
 * Produced at the Lawrence Livermore National Laboratory
 *
 * LLNL-CODE-746361
 *
 * All rights reserved. See COPYRIGHT for details.
 *
 * This file is part of the GEOSX Simulation Framework.
 *
 * GEOSX is a free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License (as published by the
 * Free Software Foundation) version 2.1 dated February 1999.
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

/*
 * MeshGeneratorBase.h
 *
 *  Created on: Oct 18, 2017
 *      Author: sherman
 */

#ifndef MESHGENERATORBASE_H_
#define MESHGENERATORBASE_H_

#include "dataRepository/ManagedGroup.hpp"
#include "codingUtilities/Utilities.hpp"
#include "common/DataTypes.hpp"

namespace geosx
{

namespace dataRepository
{
namespace keys
{
string const coarseningRatio = "coarseningRatio";
}
}

class NodeManager;
class DomainPartition;

class MeshGeneratorBase : public dataRepository::ManagedGroup
{
public:
  explicit MeshGeneratorBase( std::string const & name,
                              ManagedGroup * const parent );

  virtual ~MeshGeneratorBase();

  static string CatalogName() { return "MeshGeneratorBase"; }

  MeshGeneratorBase() = default;
  MeshGeneratorBase( MeshGeneratorBase const & ) = default;
  MeshGeneratorBase( MeshGeneratorBase &&) = default;
  MeshGeneratorBase& operator=( MeshGeneratorBase const & ) = default;
  MeshGeneratorBase& operator=( MeshGeneratorBase&& ) = default;

  virtual void GenerateElementRegions( DomainPartition& domain ) = 0;

  virtual void GenerateMesh( dataRepository::ManagedGroup * const domain ) = 0;

  // virtual void GenerateNodesets( xmlWrapper::xmlNode const & targetNode,
  //                                NodeManager * nodeManager ) = 0;

  virtual void GetElemToNodesRelationInBox ( const std::string& elementType,
                                             const int index[],
                                             const int& iEle,
                                             int nodeIDInBox[],
                                             const int size) = 0;

  virtual void RemapMesh ( dataRepository::ManagedGroup * const domain ) = 0;

  /*!
   * @brief Generate aggregates on a domain
   * @param[in] domain the domain to be coarsed
   */
  void GenerateAggregates( DomainPartition& domain);

  int m_delayMeshDeformation = 0;

  using CatalogInterface = cxx_utilities::CatalogInterface< MeshGeneratorBase, std::string const &, ManagedGroup * const >;
  static CatalogInterface::CatalogType& GetCatalog();
protected:
  real64 m_coarseningRatio;

};
}

#endif /* MESHGENERATORBASE_H_ */
