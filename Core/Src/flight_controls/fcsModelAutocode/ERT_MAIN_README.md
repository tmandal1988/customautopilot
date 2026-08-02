# ERT MAIN SAMPLE
---
```
//
// File: ert_main.cpp
//
// Code generated for Simulink model 'fcsModel'.
//
// Model version                  : 7.9
// Simulink Coder version         : 25.1 (R2025a) 21-Nov-2024
// C/C++ source code generated on : Fri Jul 31 11:50:47 2026
//
// Target selection: ert.tlc
// Embedded hardware selection: ARM Compatible->ARM Cortex-M
// Code generation objectives:
//    1. Execution efficiency
//    2. RAM efficiency
//    3. Traceability
// Validation result: All passed
//
#include <stdio.h>              // This example main program uses printf/fflush
#include "fcsModel.h"                  // Model header file

// External inputs (root inport signals)
static fcsModel::ExtUPointer_fcsModel_T fcsModel_ExtUPointer;// '<Root>/stateEstimate' 

// instance parameters
static fcsModel::InstP_fcsModel_T fcsModel_InstP{
  // Variable: fcsParamsArg
  //  Referenced by: '<Root>/fcsParams'

  busFcsParams{
    busInnerLoopCtrlParams{
      busAttCtrlParams{
        {
          {
            busPidParams{
              0.0,
              0.0,
              0.0,
              0.0,
              0.0,
              0.0,

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } }
            }, busPidParams{
              0.0,
              0.0,
              0.0,
              0.0,
              0.0,
              0.0,

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } }
            }, busPidParams{
              0.0,
              0.0,
              0.0,
              0.0,
              0.0,
              0.0,

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } }
            } }
        }
        ,

        {
          {
            busSignalConditioningParams{
              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },
              busSecondOrderFilterParam{
                0.0,
                0.0
              }
            }, busSignalConditioningParams{
              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },
              busSecondOrderFilterParam{
                0.0,
                0.0
              }
            }, busSignalConditioningParams{
              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },
              busSecondOrderFilterParam{
                0.0,
                0.0
              }
            } }
        }
        ,

        {
          {
            busSignalConditioningParams{
              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },
              busSecondOrderFilterParam{
                0.0,
                0.0
              }
            }, busSignalConditioningParams{
              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },
              busSecondOrderFilterParam{
                0.0,
                0.0
              }
            }, busSignalConditioningParams{
              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },
              busSecondOrderFilterParam{
                0.0,
                0.0
              }
            } }
        }
      },
      busAngRateCtrlParams{
        {
          {
            busPidParams{
              0.0,
              0.0,
              0.0,
              0.0,
              0.0,
              0.0,

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } }
            }, busPidParams{
              0.0,
              0.0,
              0.0,
              0.0,
              0.0,
              0.0,

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } }
            }, busPidParams{
              0.0,
              0.0,
              0.0,
              0.0,
              0.0,
              0.0,

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } }
            } }
        }
        ,

        {
          {
            busSignalConditioningParams{
              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },
              busSecondOrderFilterParam{
                0.0,
                0.0
              }
            }, busSignalConditioningParams{
              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },
              busSecondOrderFilterParam{
                0.0,
                0.0
              }
            }, busSignalConditioningParams{
              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },
              busSecondOrderFilterParam{
                0.0,
                0.0
              }
            } }
        }
        ,

        {
          {
            busSignalConditioningParams{
              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },
              busSecondOrderFilterParam{
                0.0,
                0.0
              }
            }, busSignalConditioningParams{
              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },
              busSecondOrderFilterParam{
                0.0,
                0.0
              }
            }, busSignalConditioningParams{
              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },
              busSecondOrderFilterParam{
                0.0,
                0.0
              }
            } }
        }
      }
    },
    busOuterLoopCtrlParams{
      busPosCtrlParams{
        {
          {
            busPidParams{
              0.0,
              0.0,
              0.0,
              0.0,
              0.0,
              0.0,

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } }
            }, busPidParams{
              0.0,
              0.0,
              0.0,
              0.0,
              0.0,
              0.0,

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } }
            }, busPidParams{
              0.0,
              0.0,
              0.0,
              0.0,
              0.0,
              0.0,

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } }
            } }
        }
        ,

        {
          {
            busSignalConditioningParams{
              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },
              busSecondOrderFilterParam{
                0.0,
                0.0
              }
            }, busSignalConditioningParams{
              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },
              busSecondOrderFilterParam{
                0.0,
                0.0
              }
            }, busSignalConditioningParams{
              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },
              busSecondOrderFilterParam{
                0.0,
                0.0
              }
            } }
        }
        ,

        {
          {
            busSignalConditioningParams{
              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },
              busSecondOrderFilterParam{
                0.0,
                0.0
              }
            }, busSignalConditioningParams{
              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },
              busSecondOrderFilterParam{
                0.0,
                0.0
              }
            }, busSignalConditioningParams{
              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },
              busSecondOrderFilterParam{
                0.0,
                0.0
              }
            } }
        }
      },
      busVelCtrlParams{
        {
          {
            busPidParams{
              0.0,
              0.0,
              0.0,
              0.0,
              0.0,
              0.0,

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } }
            }, busPidParams{
              0.0,
              0.0,
              0.0,
              0.0,
              0.0,
              0.0,

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } }
            }, busPidParams{
              0.0,
              0.0,
              0.0,
              0.0,
              0.0,
              0.0,

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } }
            } }
        }
        ,

        {
          {
            busSignalConditioningParams{
              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },
              busSecondOrderFilterParam{
                0.0,
                0.0
              }
            }, busSignalConditioningParams{
              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },
              busSecondOrderFilterParam{
                0.0,
                0.0
              }
            }, busSignalConditioningParams{
              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },
              busSecondOrderFilterParam{
                0.0,
                0.0
              }
            } }
        }
        ,

        {
          {
            busSignalConditioningParams{
              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },
              busSecondOrderFilterParam{
                0.0,
                0.0
              }
            }, busSignalConditioningParams{
              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },
              busSecondOrderFilterParam{
                0.0,
                0.0
              }
            }, busSignalConditioningParams{
              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },
              busSecondOrderFilterParam{
                0.0,
                0.0
              }
            } }
        }
        ,

        {
          {
            busSignalConditioningParams{
              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },
              busSecondOrderFilterParam{
                0.0,
                0.0
              }
            }, busSignalConditioningParams{
              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },
              busSecondOrderFilterParam{
                0.0,
                0.0
              }
            }, busSignalConditioningParams{
              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } },
              busSecondOrderFilterParam{
                0.0,
                0.0
              }
            } }
        }
        ,

        { { 0.0, 0.0, 0.0 } },

        { { 0.0, 0.0, 0.0 } },

        { { 0.0, 0.0, 0.0 } },
        0.0,

        { { 0.0, 0.0 } },

        { { 0.0, 0.0 } },

        { { 0.0, 0.0 } },
        0.0
      },
      busXyBodyAccelCtrlParams{
        {
          {
            busPidParams{
              0.0,
              0.0,
              0.0,
              0.0,
              0.0,
              0.0,

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } }
            }, busPidParams{
              0.0,
              0.0,
              0.0,
              0.0,
              0.0,
              0.0,

              { { 0.0, 0.0 } },

              { { 0.0, 0.0 } }
            } }
        }
        ,

        { { 0.0, 0.0 } }
      },
      busZaccelCtrlParams{
        busPidParams{
          0.0,
          0.0,
          0.0,
          0.0,
          0.0,
          0.0,

          { { 0.0, 0.0 } },

          { { 0.0, 0.0 } }
        },
        busSignalConditioningParams{
          { { 0.0, 0.0 } },

          { { 0.0, 0.0 } },

          { { 0.0, 0.0 } },

          { { 0.0, 0.0 } },
          busSecondOrderFilterParam{
            0.0,
            0.0
          }
        },
        busSignalConditioningParams{
          { { 0.0, 0.0 } },

          { { 0.0, 0.0 } },

          { { 0.0, 0.0 } },

          { { 0.0, 0.0 } },
          busSecondOrderFilterParam{
            0.0,
            0.0
          }
        },
        0.0
      }
    },
    busSysIdInjectionParams{
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,
      0.0,

      { { 0.0, 0.0 } },

      { { 0.0, 0.0 } }
    }
  }
};

static fcsModel fcsModel_Obj{ &fcsModel_ExtUPointer, &fcsModel_InstP };// Instance of model class 

//
// Associating rt_OneStep with a real-time clock or interrupt service routine
// is what makes the generated code "real-time".  The function rt_OneStep is
// always associated with the base rate of the model.  Subrates are managed
// by the base rate from inside the generated code.  Enabling/disabling
// interrupts and floating point context switches are target specific.  This
// example code indicates where these should take place relative to executing
// the generated code step function.  Overrun behavior should be tailored to
// your application needs.  This example simply sets an error status in the
// real-time model and returns from rt_OneStep.
//
void rt_OneStep(void);
void rt_OneStep(void)
{
  static boolean_T OverrunFlag{ false };

  // Disable interrupts here

  // Check for overrun
  if (OverrunFlag) {
    return;
  }

  OverrunFlag = true;

  // Save FPU context here (if necessary)
  // Re-enable timer or interrupt here
  // Set model inputs here

  // Step the model
  fcsModel_Obj.step();

  // Get model outputs here

  // Indicate task complete
  OverrunFlag = false;

  // Disable interrupts here
  // Restore FPU context here (if necessary)
  // Enable interrupts here
}

//
// The example main function illustrates what is required by your
// application code to initialize, execute, and terminate the generated code.
// Attaching rt_OneStep to a real-time clock is target specific. This example
// illustrates how you do this relative to initializing the model.
//
int_T main(int_T argc, const char *argv[])
{
  // Unused arguments
  (void)(argc);
  (void)(argv);

  // Initialize model
  fcsModel_Obj.initialize();

  // Attach rt_OneStep to a timer or interrupt service routine with
  //  period 0.004 seconds (base rate of the model) here.
  //  The call syntax for rt_OneStep is
  //
  //   rt_OneStep();

  printf("Warning: The simulation will run forever. "
         "Generated ERT main won't simulate model step behavior. "
         "To change this behavior select the 'MAT-file logging' option.
");
  fflush((nullptr));
  while (1) {
    //  Perform application tasks here
  }

  // The option 'Remove error status field in real-time model data structure'
  //  is selected, therefore the following code does not need to execute.


  // Terminate model
  fcsModel_Obj.terminate();
  return 0;
}

//
// File trailer for generated code.
//
// [EOF]
//

```