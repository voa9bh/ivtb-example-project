package buildinfo

class BuildInfo {
  static Properties props = null

  static String getAppProperty(String propertyName) {
    if (props == null) {
      String className = BuildInfo.class.canonicalName

      String packageName = className[0..(className.lastIndexOf('.') - 1)]

      packageName = '/' + packageName.replaceAll('\\.', '/')

      InputStream is = BuildInfo.class.getResourceAsStream("${packageName}/buildinfo.properties")
      props = new Properties()
      props.load(is)
    }
    String returnValue = props.getProperty(propertyName)

    if (returnValue != null && returnValue.startsWith('${')) {
      returnValue = null
    }

    return returnValue
  }

  /**
   * function returns the date of the build
   **/
  static String getBuildDate() {
    return getAppProperty('app.build.date') ?: '<unknown>'
  }

  /**
   * getBranchName returns the branch name which was used for build
   **/
  static String getBranchName() {
    return getAppProperty('app.branch.name') ?: '<unknown>'
  }

  /**
   * getCommitHash returns the CommitHash which was the base of the build
   **/
  static String getCommitHash() {
    return getAppProperty('app.commit.hash') ?: '<unknown>'
  }

  /**
   * getVersion returns the next available version
   *
   * If the build is not on a oficial version the tag is expandad by an number
   * and a part of the commit hash.
   **/
  static String getVersion() {
    return getAppProperty('app.version') ?: '0.0.0'
  }

  /**
   * repositoryWasCleanAtBuildTime returns the status of the repository at build time
   *
   * If this function returns fals the code can be diffrent to the code commited by
   * with the hash getCommitHash!
   **/
  static boolean repositoryWasCleanAtBuildTime() {
    boolean repositoryWasCleanAtBuildTime = getAppProperty('app.is.repo.clean') == "true"

    return repositoryWasCleanAtBuildTime
  }

  /**
   * getCommitDate returns the date of the commit
   **/
  static String getCommitDate() {
    return getAppProperty('app.commit.date') ?: '<unknown>'
  }

  /**
   * isReleasedVersion returns true if the commit is equals to the next available
   * version tag
   **/
  static boolean isReleasedVersion() {
    boolean isReleasedVersion = getAppProperty('app.is.on.tag') == "true"

    return isReleasedVersion
  }
}
