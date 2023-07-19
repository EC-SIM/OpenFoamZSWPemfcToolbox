// Code changed and adapted to pemfc (J.P.Kone, 19.07.2016)

// for coversion to openfoam-4.0
// call to 'internalField()' is changed to 'ref()'

// fuel side species diffusivities
//
// algorithm
// For a fixed-diffusivity model we only need to assign the diffusivity
// Otherwise we must use a binary model, possibly modified for porosity
// For a binary model we calculate pairwise diffusivities D[a,b],
// and calculate species diffusivity in mixture as [Wilke 1950]
//
//                   1 - x[a]
//     D[a] = ---------------------- , where x[a] is mole fraction of specie a
//            sum_{b!=a}(x[b]/D[a,b]
//
// We consider "a" as the diffusing specie and "b" as the background specie --
// this is only relavent to binary porous models invoking knudsen diffusion.
//


forAll(fuelSpecies, a)
{
    fuelDiff = 0;         //diffusivity field passed to/from model

    //diffSpFuel[a] is diffusivity field for specie a, to be used in YEqn
    scalarField& diffSpFuelIn = diffSpFuel[a].ref();
    diffSpFuelIn = 0;

    if(fuelSpecies[a].name() != fuelInertSpecie)
    {
        Info<< nl << "fuelSpecies " << fuelSpecies[a].name() << nl;
    
        forAll(fuelDiffModels, m)
        {
            /*Info<< nl << "fuelDiffModels[" << m << "]" << nl;
            Info<< "  zone size = "
                 << fuelDiffModels[m]->cells().size() << nl;*/

            if(fuelDiffModels[m]->isFixed())
            {
                //Info << "fixed: a m = " << a << "  " << m << "  " << nl;
                //Info << "  zone size = "
                //     << fuelDiffModels[m]->cells().size() << nl;

                fuelDiff = 0;
                fuelDiffModels[m]->evaluate();
                //Info << "hkr ->writeData() ";
                //fuelDiffModels[m]->writeData();

                // copy calc'd zone diffusivity from fuelDiff to diffSpFuel[a]
                forAll(fuelDiffModels[m]->cells(), I)
                {
                    label iCell = fuelDiffModels[m]->cells()[I];
                    diffSpFuelIn[iCell] = fuelDiff[iCell];

                  /********* -hkr ***************\
                    Info<< "iCell fuelDiff: " 
                        << iCell << "  "
                        << fuelDiff[iCell] << nl;
                  \********** hkr- **************/
                }

              /********* -hkr ********************************\
                Info << "diffSpFuelIn [min mean max] = ["
                     << Foam::min(diffSpFuelIn) << "  "
                     << Foam::average(diffSpFuelIn) << "  "
                     << Foam::max(diffSpFuelIn) << "]" << nl;
              \********** hkr- *******************************/
            }
            else if(!fuelDiffModels[m]->isBinary())
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
                scalarField sumA(fuelDiff.size(), 0);

                //Info<< "min max sumA = "
                //    << gMin(sumA) << "  " << gMax(sumA) << nl;

                forAll(fuelSpecies,b)
                {
                    if (b != a)
                    {
                        //Info<< "m  a b = "
                        //    << m << "  " << a << " " << b << nl; 

                        fuelDiff = 0;
                        fuelDiffModels[m]->setSpecies
                        (
                            fuelSpecies[a].name(),
                            fuelSpecies[b].name()
                        );
                        fuelDiffModels[m]->evaluate();
                        //Info << "hkr ->writeData() ";
                        //fuelDiffModels[m]->writeData(); //Commented S. Schneider

                        //Info<< "min mean max fuelDiff:"
                        //    << gMin(fuelDiff) << "  "
                        //    << gAverage(fuelDiff) << "  "
                        //    << gMax(fuelDiff) << nl;
                        //Info<< endl;

                        //Info<< "  zone size = "
                        //    << fuelDiffModels[m]->cells().size() << nl;

                        //forAll(fuelDiff, I)
                        forAll(fuelDiffModels[m]->cells(), I)
                        {
                            label iCell = fuelDiffModels[m]->cells()[I];
                            if(fuelDiff[iCell] != 0) //hkr: surely this must be!
                            {
                                sumA[iCell] += Xfuel[b][iCell]/fuelDiff[iCell];

                              /************ -hkr ******************\
                                Info<< "iCell xfuel fuelDiff sumA: " 
                                    << iCell << "  "
                                    << xFuel[b][iCell] << "  "
                                    << fuelDiff[iCell] << "  "
                                    << sumA[iCell] << nl;
                              \************* hkr- *****************/
                            }
                        }
                    }
                }

                // diffSpFuelIn[zone] <-- (1-xFuel[a])/sumA
                forAll(fuelDiffModels[m]->cells(), I)
                {
                    label iCell = fuelDiffModels[m]->cells()[I];
                    if (sumA[iCell] != 0)
                    {
                       diffSpFuelIn[iCell] = (1 - Xfuel[a][iCell])/sumA[iCell];
                    }
                }
                //Info << "m a = " << m << "  " << a << "  " << nl;
              /****************************************************\
                Info << "diffSpFuelIn [min mean max] = ["
                     << Foam::min(diffSpFuelIn) << "  "
                     << Foam::average(diffSpFuelIn) << "  "
                     << Foam::max(diffSpFuelIn) << "]" << nl;
              \****************************************************/

            } //isBinary
        } //m

      /**************** -hkr ******************************\
        Info << "a = " << a << "   diffSpFuelIn: " << nl;
        Info << diffSpFuelIn << nl;
      \***************** hkr- *****************************/

        Info<< nl
             << "diffSpFuel["
             << fuelSpecies[a].name()
             << "]"
             << nl
             << "    internalField min mean max = "
             << Foam::gMin(diffSpFuelIn) << "  "      //S.Keuler, Bugfix for consistent output (parallel simulation) [next three lines]
             << Foam::gAverage(diffSpFuelIn) << "  "
             << Foam::gMax(diffSpFuelIn)
	     << nl;

        diffSpFuel[a].correctBoundaryConditions();
    } //!inert
} //a


