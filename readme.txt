#environment setup
JAVA_HOME=/usr/jdk/latest
JAVA_INCLUDE_PATH=$JAVA_HOME/include/
JAVA_INCLUDE_PATH2=$JAVA_HOME/include/solaris/
PATH=/usr/local/gcc/4.7.2/bin:/opt/csw/i386-pc-solaris2.10/bin:/opt/csw/lib:/opt/csw/bin:/usr/local/lib:/usr/local/bin:/usr/bin:/usr/bin:/usr/sbin:/usr/local/bin:/usr/sfw/bin:/usr/local/sbin:/opt/puppet/bin:$JAVA_HOME/bin
LD_LIBRARY_PATH=/usr/local/gcc/4.7.2/lib/gcc/i386-pc-solaris2.10/4.7.2/4:/usr/local/gcc/4.7.2/lib:/opt/csw/i386-pc-solaris2.10/bin:/opt/csw/lib:/opt/csw/bin:/usr/local/lib/:/usr/local/bin/:/usr/bin/:/usr/local/gcc/4.7.2/bin:/usr/local/lib/amd64:/usr/local/gcc/4.7.2/lib/amd64:/opt/csw/lib/amd64
export JAVA_HOME JAVA_INCLUDE_PATH JAVA_INCLUDE_PATH2 LD_LIBRARY_PATH PATH

cd $HONEST_PROFILER_PROJECT
cmake .
make