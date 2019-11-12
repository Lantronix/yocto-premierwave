DESCRIPTION = "Simple Java hello world application"
SECTION = "examples"
LICENSE = "MIT"

LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

RDEPENDS_${PN} = "openjre-8"

SRC_URI = "file://HelloWorld.java file://manifest.txt"

S = "${WORKDIR}"

inherit java-library

app = "java-helloworld"

do_compile() {
    mkdir -p build
    javac -d build `find . -name "*.java"`
#   echo `pwd`
#   fastjar cf ${JARFILENAME} manifest.txt -C build .
    mv -f build/*.class .
    jar cvfm ${JARFILENAME} manifest.txt `find . -name "*.class"`

}

do_install() {
    install -D -m 0755 ${B}/${JARFILENAME} ${D}/bin/${app}.jar
}

BBCLASSEXTEND = "native"
