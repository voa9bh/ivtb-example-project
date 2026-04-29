@Library('jenkins-pipeline-library')_
import bosch.bh.jenkins.Parameter
Parameter parameter = new Parameter()
parameter.context = this

//todo: set projectName, correct relativeArtifactoryPath, email addresses and coverage requirements
//Name of the Testprogram (should be same like in settings.gradle)
parameter.projectName = 'YourProgramName'

//major.minor.patch - * will be replaced by TAG.  (if releas/development minor+1 & patch=0, if bugfix patch+1)
parameter.releaseVersionPattern ='1.*.*'

//Jenkins buildinfo email
parameter.emailListDevelopers ='michael.maaz@de.bosch.com;Andreas.Vorgeitz@de.bosch.com'
parameter.emailListUsers = 'michael.maaz@de.bosch.com;Tiphanie.Deniaux@de.bosch.com;Martin.Eberle@de.bosch.com;Benjamin.Hummel@de.bosch.com;Adrian.Koch@de.bosch.com;Michael.Kolb@de.bosch.com;Mathias.Schoell@de.bosch.com;Vincenz.Vogler@de.bosch.com;Andreas.Vorgeitz@de.bosch.com'

//Coverage
parameter.minLineCoverage = 49
parameter.minBranchCoverage = 1

//ArifactoryHome:
parameter.relativeArtifactoryPath='bosch/bh/testprograms/yourPackage/' //must be like package in build.gradle with / instead of . !!!!!

//No Codenarc for Test code
parameter.doNotCheckTests = true //because codeNarc rules for Tests are to much at the moment. Therefore we need other rules

/* Optional parameters (with their default values)
parameter.minMethodCoverage=1
parameter.minComplexityCoverage=1
parameter.minInstructionCoverage=1
parameter.minClassCoverage=1
parameter.homePath = './'
parameter.artifactoryHome = 'bhp-eat-spexjava-local/' //change this only, if you know what you do!
parameter.timeoutHours = 1
*/


spexTestprogramPipeline(parameter)

