#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

#define StartTest -4E-7
#define StopTest 4E-7

formatExpImpResp(int numPnts, double *timeValues, double *voltageValues,
	int samplingFrequencyHz)
{
double *newTimeValues;
double timeAtMaxIntensity;
double minTime = DBL_MIN;
double maxTime = -DBL_MIN;
double maxVoltage = -DBL_MIN;
double stepSize;
int i, maxVoltageIndex, numSteps;
int startIndex, stopIndex;

/* find the max voltage */

	for (i = 0; i < numPnts; i++) {
		if (maxVoltage < voltageValues[i]) {
			maxVoltage = voltageValues[i];
			maxVoltageIndex = i;
			}
		}

	fprintf(stderr, "index %d max voltage %e\n", maxVoltageIndex, maxVoltage);

/* normalize the voltages */

	for (i = 0; i < numPnts; i++)
		voltageValues[i] /= maxVoltage;


/*
 * center the time axis around the max intensity. the matlab code recalculated
 * the index of the maximum value, but the normalization shouldn't change
 * that, so I skipped it. note that I have to save the time associated with
 * the max intensity, because in the time array, it goes to 0 as I do the
 * centering.
 */

	timeAtMaxIntensity = timeValues[maxVoltageIndex];

	for (i = 0; i < numPnts; i++)
		timeValues[i] = timeValues[i] - timeAtMaxIntensity;

/* now find the min and max times */

	for (i = 0; i < numPnts; i++) {
		if (minTime > timeValues[i]) minTime = timeValues[i];

		if (maxTime < timeValues[i]) maxTime = timeValues[i];
		}

	fprintf(stderr, "min time %e max time %e\n", minTime, maxTime);
/* re-sample the data to match the Field II sampling frequency */

	fprintf(stderr, "sampling %d\n", samplingFrequencyHz);
	stepSize = 1.0 / samplingFrequencyHz;

	fprintf(stderr, "diff %e\n", maxTime - minTime);
	numSteps = (int )ceil(((maxTime - minTime) * samplingFrequencyHz));

	fprintf(stderr, "step size %e numSteps %d\n", stepSize, numSteps);

	if ((newTimeValues = (double *)malloc(sizeof(double) * numSteps)) == NULL) {
		fprintf(stderr, "in readExpData, couldn't allocate space for new times\n");
		return(0);
		}

	for (i = 0; i < numSteps; i++) {
		newTimeValues[i] = minTime + i * stepSize;
		}

/* find the indices of NewTime to use for the Field II impulse response */

	for (i = 0; i < numSteps; i++) {
		if (newTimeValues[i] > StartTest) {
			startIndex = i;
			break;
			}
		}

	for (i = 0; i < numSteps; i++)
		if (newTimeValues[i] > StopTest) {
			stopIndex = i;
			break;
			}

	fprintf(stderr, "startIndex %d, stopIndex %d\n", startIndex, stopIndex);
}
