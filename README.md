# SPEX TL Example Project

## Customize it for your project

1. **Get the source**
    > clone or copy all files into new folder with the name of your project. Remove the .git folder.
2. **Modify the settings.gradle**
    > Set the root project name rootproject.name = 'Project name'
3. **Modify build.gradle** 
    * set group to a valid package name e.g. bosch.bh.mynewppg
    * set mainClassName to the new mainclass path e.g. bosch.bh.mynewppg.main.Main
4. **Modify Jenkinsfile**
    * set projectName, email-lists and relative artifactory path 
5. **Open Project in IDE (intelliJ) and refactor package name**
    * Open Ide and open project File->Open navigate to your copy of the project and open build.gradle as project
    * To test the project initial run Tastks->verification->test via Gradle
    * rename package your.name.here in src with the the refactor function of intelliJ to bosch.bh.mynewppg
    * rerun the test
6. **Create git repository in Bitbucket and follow the instructions for existing code**