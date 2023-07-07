# OpenFoamZSWPemfcToolbox

## Developed within the project AutoStack Industrie - ASI

The toolbox was developed within the ASI-Project. Funded by the German Federal Ministry of Transport (grant number: 03B10103), the “AutoStack-Industrie” joint project with 10 industry partners was launched to develop a high-performance fuel cell stack suitable for series production for automotive applications and was successfully completed in 2022.

Related links:

https://www.zsw-bw.de/en/newsroom/news/news-detail/news/detail/News/fuel-cell-powered-e-mobility-autostack-industry-project-sets-the-stage-for-mass-fuel-cell-manufa.html

https://www.now-gmbh.de/en/projectfinder/autostack-industrie/

## OpenFOAM v7
The pemfcFOAM solver is based on OpenFOAM v7 that can be downloaded under: https://openfoam.org/version/7/



## This toolbox is base on pemfcSinglePhaseModel-4.0

The development is based on the pemfcSinglePhaseModel-4.0, which is published under CC BY 4.0 under: https://unnc.digitalcommonsdata.com/datasets/3gz7pxznzn/1 (DOI: 10.17632/3gz7pxznzn.1)

This model is a 3-D, non-isothermal and single-phase flow model for simulation of PEM fuel cells. Assumptions of the electrochemical model are:
- steady-state operating condition
- gas flow: laminar and incompressible, ideal gas
- fuel cell components: isotropic and homogenous
- electrochemical reaction is modelled at cathode catalyst layer membran interface (2D)
- membrane: fully humidified, impermeable to reactant gases
- anode: avtivation overpotential and mass transport losses are neglegted
- ohmic heatimg not considered 


## Features/Modification compared to pemfcSinglePhaseModel-4.0
The pemfcSinglePhaseModel-4.0 was updated from OpenFOAM v4 to OpenFOAM v7. Some inconsistencies in the code concerning modelling of porous media, gas mixture on anode side, source term of energy equation and possibility for parallel calculation has to be fixed. 

New implentation are:
- more realistic membrane humidity: average of anode and cathode relative humidity
- considering anode activation overpotential
- simplified Butler-Vollmer equation for modelling of activation overpotential of anode and cathode
- implementation of hydrogen crossover
- implementation of "postProcessing" functunality of OpenFOAM
- Implementation of "fvOption" functionality of OpenFOAM (e.g. fixed values can be defined)


## Getting started
<!---
# Features

## Supported Platforms


## Prerequisites

## Setting up OpenFoamZSWPemfcToolbox

# Documentation

# Feedback

## Reporting Bugs

## Development / How to contribute

--- ? Include links to speparate files ("Contribution guidelines" and "Roadmap") ? ---

# Acknowledgements

## Funding

## Cotributors

# Copyright and License
--->

## OpenFOAM Licence (GPLv3)
"OpenFOAM is distributed by the OpenFOAM Foundation and is freely available and open source, licensed under the GNU General Public Licence.

There are two main elements to the GPL, designed to prevent open source software being exploited by their inclusion within non-free, closed sourced software products:

1. Software that includes source code licensed under the GPL inherits the GPL licence.
2. If compiled binaries of software licensed under GPL are distributed, the source code must also be made available by the distributor.

These aspects of the licence discourage exploitation, because if a closed sourced software product that includes open source software is sold for a fee, anyone purchasing the product could demand the source code and redistribute it for free.

Apart from this, the licence is designed to offer freedom, in particular it does not force users of the software to make modifications or developments publicly available. That means that software such as OpenFOAM can be used as the basis of in-house software." [https://openfoam.org/licence/]

# Contact 📫

https://www.zsw-bw.de/en/research/fuel-cells/topics/modelling-and-simulation.html

ec-sim@zsw-bw.de
