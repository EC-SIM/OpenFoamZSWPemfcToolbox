// Code changed and adapted to pemfc (J.P.Kone, 19.07.2016)
// for coversion to openfoam-4.0
// call to 'internalField()' is changed to 'ref()'

// air side species diffusivities
//
// algorithm
// For a fixed-diffusivity model we only need to assign the diffusivity
// Otherwise we must use a binary model, possibly modified for porosity
// For a binary model we calculate pairwise diffusivities D[a,b],
// and calculate species diffusivity in mixture as [Wilke 1950]
//
//                   1 - x[a]
//     D[a] = ---------------------- , where x[a] is mole fraction of species a
//            sum_{b!=a}(x[b]/D[a,b]
//
// We consider "a" as the diffusing species and "b" as the background species --
// this is only relevant to binary porous models invoking knudsen diffusion.
//


forAll(airSpecies, a)
{
    airDiff = 0;         //diffusivity field passed to/from model

    //diffSpAir[a] is diffusivity field for specie a, to be used in YEqn
    scalarField& diffSpAirIn = diffSpAir[a].ref();
    diffSpAirIn = 0;

    if(airSpecies[a].name() != airInertSpecie)
    {
        Info<< nl << "airSpecies " << airSpecies[a].name() << nl;
    
        forAll(airDiffModels, m)
        {
            /*Info<< nl << "airDiffModels[" << m << "]" << nl;
            Info << "  zone size = "
                 << airDiffModels[m]->cells().size() << nl;*/

            if(airDiffModels[m]->isFixed())
            {
                //Info << "fixed: a m = " << a << "  " << m << "  " << nl;

                airDiff = 0;
                airDiffModels[m]->evaluate();
                //Info << "hkr ->writeData() ";
                //airDiffModels[m]->writeData();

                // copy calculated zone diffusivity from airDiff to diffSpAir[a]
                forAll(airDiffModels[m]->cells(), I)
                {
                    label iCell = airDiffModels[m]->cells()[I];
                    diffSpAirIn[iCell] = airDiff[iCell];

                    //Info<< "iCell airDiff: " 
                    //    << iCell << "  "
                    //    << airDiff[iCell] << nl;
                }

                //Info << "diffSpAirIn [min mean max] = ["
                //     << Foam::min(diffSpAirIn) << "  "
                //     << Foam::average(diffSpAirIn) << "  "
                //     << Foam::max(diffSpAirIn) << "]" << nl;
            }
            else if(!airDiffModels[m]->isBinary())
            {
                 Info<< "ERROR: multicomponent diffusivity "
                     << "requires fixed or binary model" << nl;
                 exit(1);
            }
            else
            {
                // species diffusivities in mixture from binary diffusivities
                // ----------------------------------------------------------
                // D_{a} = (1-x[a])/sum_{b!=a}(x[b]/D[a,b])

                // pairwise binary diff calculation and accumulation
                // -------------------------------------------
                //initialize sum(x[b]/D[ab]
                scalarField sumA(airDiff.size(), 0);

                //Info<< "min max sumA = "
                //    << gMin(sumA) << "  " << gMax(sumA) << nl;

                forAll(airSpecies,b)
                {
                    if (b != a)
                    {
                        //Info<< "m  a b = "
                        //    << m << "  " << a << " " << b << nl; 

                        airDiff = 0;
                        airDiffModels[m]->setSpecies
                        (
                            airSpecies[a].name(),
                            airSpecies[b].name()
                        );
                        airDiffModels[m]->evaluate();
                        //Info << "hkr ->writeData() ";
                        //airDiffModels[m]->writeData();

                        //Info<< "min mean max airDiff:"
                        //    << gMin(airDiff) << "  "
                        //    << gAverage(airDiff) << "  "
                        //    << gMax(airDiff) << nl;
                        //Info<< endl;

                        //Info<< "  zone size = "
                        //    << airDiffModels[m]->cells().size() << nl;

                        forAll(airDiffModels[m]->cells(), I)
                        {
                            label iCell = airDiffModels[m]->cells()[I];
                            if(airDiff[iCell] != 0) //hkr: surely this must be!
                            {
                                sumA[iCell] += Xair[b][iCell]/airDiff[iCell];

                              /************ -hkr ******************\
                                Info<< "iCell xair airDiff sumA: " 
                                    << iCell << "  "
                                    << xAir[b][iCell] << "  "
                                    << airDiff[iCell] << "  "
                                    << sumA[iCell] << nl;
                              \************* hkr- *****************/
                            }
                        }
                    }
                }

                // diffSpAirIn[zone] <-- (1-xAir[a])/sumA
                forAll(airDiffModels[m]->cells(), I)
                {
                    label iCell = airDiffModels[m]->cells()[I];
                    if (sumA[iCell] != 0)
                    {
                        diffSpAirIn[iCell] = (1 - Xair[a][iCell])/sumA[iCell];
                    }
                }
                //Info << "m a = " << m << "  " << a << "  " << nl;
              /****************************************************\
                Info << "diffSpAirIn [min mean max] = ["
                     << Foam::min(diffSpAirIn) << "  "
                     << Foam::average(diffSpAirIn) << "  "
                     << Foam::max(diffSpAirIn) << "]" << nl;
              \****************************************************/

            } //isBinary
        } //m

      /**************** -hkr ******************************\
        Info << "a = " << a << "   diffSpAir[a]: " << nl;
        Info << diffSpAir[a] << nl;
      \***************** hkr- *****************************/

        Info<< nl
             << "diffSpAir["
             << airSpecies[a].name()
             << "]"
             << nl
             << "    internalField min mean max = "
             << Foam::gMin(diffSpAirIn) << "  "    //S.Keuler, Bugfix for consistent output (parallel simulation) [next three lines]
             << Foam::gAverage(diffSpAirIn) << "  "
             << Foam::gMax(diffSpAirIn)
             << nl;

        diffSpAir[a].correctBoundaryConditions();
    } //!inert
} //a


