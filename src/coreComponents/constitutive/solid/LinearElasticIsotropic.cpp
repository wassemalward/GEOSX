/*
 * ------------------------------------------------------------------------------------------------------------
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (c) 2018-2020 Lawrence Livermore National Security LLC
 * Copyright (c) 2018-2020 The Board of Trustees of the Leland Stanford Junior University
 * Copyright (c) 2018-2020 Total, S.A
 * Copyright (c) 2019-     GEOSX Contributors
 * All rights reserved
 *
 * See top level LICENSE, COPYRIGHT, CONTRIBUTORS, NOTICE, and ACKNOWLEDGEMENTS files for details.
 * ------------------------------------------------------------------------------------------------------------
 */

/**
 *  @file LinearElasticIsotropic.cpp
 */

#include "LinearElasticIsotropic.hpp"

namespace geosx
{
using namespace dataRepository;
namespace constitutive
{

LinearElasticIsotropic::LinearElasticIsotropic( std::string const & name, Group * const parent ):
  SolidBase( name, parent ),
  m_defaultBulkModulus(),
  m_defaultShearModulus(),
  m_bulkModulus(),
  m_shearModulus()
{
  registerWrapper( viewKeyStruct::defaultBulkModulusString, &m_defaultBulkModulus )->
    setApplyDefaultValue( -1 )->
    setInputFlag( InputFlags::OPTIONAL )->
    setDescription( "Elastic Bulk Modulus Parameter" );

  registerWrapper( viewKeyStruct::defaultShearModulusString, &m_defaultShearModulus )->
    setApplyDefaultValue( -1 )->
    setInputFlag( InputFlags::OPTIONAL )->
    setDescription( "Elastic Shear Modulus Parameter" );

  registerWrapper< real64 >( viewKeyStruct::defaultYoungsModulusString )->
    setApplyDefaultValue( -1 )->
    setInputFlag( InputFlags::OPTIONAL )->
    setDescription( "Elastic Young's Modulus." );

  registerWrapper< real64 >( viewKeyStruct::defaultPoissonRatioString )->
    setApplyDefaultValue( -1 )->
    setInputFlag( InputFlags::OPTIONAL )->
    setDescription( "Poisson's ratio" );

  registerWrapper( viewKeyStruct::bulkModulusString, &m_bulkModulus )->
    setApplyDefaultValue( -1 )->
    setDescription( "Elastic Bulk Modulus Field" );

  registerWrapper( viewKeyStruct::shearModulusString, &m_shearModulus )->
    setApplyDefaultValue( -1 )->
    setDescription( "Elastic Shear Modulus" );
}


LinearElasticIsotropic::~LinearElasticIsotropic()
{}


void LinearElasticIsotropic::setDefaultBulkModulus( real64 const bulkModulus )
{
  m_defaultBulkModulus = bulkModulus;
  this->getWrapper< decltype(m_bulkModulus) >( viewKeyStruct::bulkModulusString )->
    setApplyDefaultValue( m_defaultBulkModulus );
}

void LinearElasticIsotropic::setDefaultShearModulus( real64 const shearModulus )
{
  m_defaultShearModulus = shearModulus;
  this->getWrapper< decltype(m_shearModulus) >( viewKeyStruct::shearModulusString )->
    setApplyDefaultValue( m_defaultShearModulus );
}


void LinearElasticIsotropic::PostProcessInput()
{

  SolidBase::PostProcessInput();

  real64 & nu = getReference< real64 >( viewKeyStruct::defaultPoissonRatioString );
  real64 & E  = getReference< real64 >( viewKeyStruct::defaultYoungsModulusString );
  real64 & K  = m_defaultBulkModulus;
  real64 & G  = m_defaultShearModulus;

  string errorCheck( "( " );
  int numConstantsSpecified = 0;
  if( nu >= 0.0 )
  {
    ++numConstantsSpecified;
    errorCheck += "nu, ";
  }
  if( E >= 0.0 )
  {
    ++numConstantsSpecified;
    errorCheck += "E, ";
  }
  if( K >= 0.0 )
  {
    ++numConstantsSpecified;
    errorCheck += "K, ";
  }
  if( G >= 0.0 )
  {
    ++numConstantsSpecified;
    errorCheck += "G, ";
  }
  errorCheck += ")";

  GEOSX_ERROR_IF( numConstantsSpecified != 2,
                  "A specific pair of elastic constants is required. Either (K,G) or (E,nu). "<<
                  "You have specified "<<errorCheck );

  if( nu >= 0.0 && E >= 0.0 )
  {
    K = E / (3 * ( 1 - 2*nu ) );
    G = E / (2 * ( 1 + nu ) );
  }
  else if( nu >= 0.0 && G >= 0.0 )
  {
    E = 2 * G * ( 1 + nu );
    K = E / (3 * ( 1 - 2*nu ) );
  }
  else if( nu >= 0 && K >= 0.0 )
  {
    E = 3 * K * ( 1 - 2 * nu );
    G = E / ( 2 * ( 1 + nu ) );
  }
  else if( E >= 0.0 && K >=0 )
  {
    nu = 0.5 * ( 1 - E /  ( 3 * K ) );
    G = E / ( 2 * ( 1 + nu ) );
  }
  else if( E >= 0.0 && G >= 0 )
  {
    nu = 0.5 * E / G - 1.0;
    K = E / (3 * ( 1 - 2*nu ) );
  }
  else if( K >= 0.0 && G >= 0.0 )
  {
    E = 9 * K * G / ( 3 * K + G );
    nu = ( 3 * K - 2 * G ) / ( 2 * ( 3 * K + G ) );
  }
  else
  {
    GEOSX_ERROR( "invalid specification for default elastic constants. "<<errorCheck<<" has been specified." );
  }

  this->getWrapper< array1d< real64 > >( viewKeyStruct::bulkModulusString )->
    setApplyDefaultValue( m_defaultBulkModulus );

  this->getWrapper< array1d< real64 > >( viewKeyStruct::shearModulusString )->
    setApplyDefaultValue( m_defaultShearModulus );
}

REGISTER_CATALOG_ENTRY( ConstitutiveBase, LinearElasticIsotropic, std::string const &, Group * const )
}
} /* namespace geosx */
