overlay_path="golden_gate"
product=`cat conf/local.conf | grep MACHINE | grep -v "#" | cut -d'"' -f2`
version=`cat ../sources/meta-lantronix/conf/machine/${product}.conf | grep LTRX_PRODUCT_VERSION | cut -d'"' -f2`
linux_dir=linux-at91
echo "Build image version" $version
echo "Target" $product
   

if [ "$1" == "sdk" ]; then
   #Build the sdk and exit
   bitbake ltrx-internal-image -c populate_sdk
   exit
fi
if [ "$1" == "ltrx-apps" ]; then
   rm -rf tmp/deploy/images/${product}/ltrx-apps-sdk.tar.xz
   rm -rf ../sources/meta-lantronix/recipes-bsp/ltrx-apps/files/ltrx-apps-sdk.tar.xz
   cd ../lantronix/
   rm -rf ltrx-apps-internal.tar.xz
   rm -rf ../sources/meta-lantronix-internal/recipes-bsp/ltrx-apps/files/ltrx-apps-internal.tar.xz
   tar -cJf ltrx-apps-internal.tar.xz ltrx-apps --exclude-vcs
   mv ltrx-apps-internal.tar.xz ../sources/meta-lantronix-internal/recipes-bsp/ltrx-apps/files/
   if [ $? != 0 ]; then
       echo "Copy tar: $? - Unsuccessful. Location mismatch or not created tar" 
       exit
   fi
   cd -
   echo "Cleaning ltrx-apps internal" 
   #bitbake ltrx-apps-internal ltrx-apps -c cleanall
fi


if [ "$1" == "overlay" ]; then
   cd ../lantronix/buildroot-2014.05/board/lantronix/${overlay_path}/overlay/
   tar -cJf ${product}-overlay.tar.xz . --exclude-vcs
   cd -
   mv ../lantronix/buildroot-2014.05/board/lantronix/${overlay_path}/overlay/${product}-overlay.tar.xz ../sources/meta-lantronix/recipes-bsp/images/files/
   if [ $? != 0 ]; then
      echo "Copy tar: $? - Unsuccessful. Location mismatch or not created tar" 
   exit
   fi
   cd ../lantronix/buildroot-2014.05/board/lantronix/overlay/
   tar -cJf overlay.tar.xz . --exclude-vcs
   cd -
   mv ../lantronix/buildroot-2014.05/board/lantronix/overlay/overlay.tar.xz ../sources/meta-lantronix/recipes-bsp/images/files/
   if [ $? != 0 ]; then
       echo "Copy tar: $? - Unsuccessful. Location mismatch or not created tar" 
       exit
   else 
      echo "cleaning ltrx image" 
      bitbake ltrx-internal-image -c cleanall
      echo "Building ltrx-image"
      bitbake ltrx-internal-image
      exit
   fi
fi

if [ "$1" == "linux-at91" ]; then
   cd ../lantronix
   echo "Taking tar of linux and It may take some time. Please wait....."
   tar -cJf ${linux_dir}.tar.xz ${linux_dir} --exclude-vcs
   cd -
   mv ../lantronix/${linux_dir}.tar.xz ../sources/meta-lantronix/recipes-kernel/linux/linux-yocto-custom/
   if [ $? != 0 ]; then                   
      echo "Copy tar: $? - Unsuccessful. Location mismatch or not created tar" 
   exit
   else
      echo "Cleaning linux and ltrx image" 
      bitbake linux-yocto-custom ltrx-image -c cleanall
      bitbake ltrx-build-image
      exit
   fi
fi

#echo "Building ltrx-apps internal"
#bitbake ltrx-apps-internal

#if [ -f "tmp/deploy/images/${product}/ltrx-apps-sdk.tar.xz" ]; then
#  echo "ltrx apps built successfuly"
#  cp tmp/deploy/images/${product}/ltrx-apps-sdk.tar.xz ../sources/meta-lantronix/recipes-bsp/ltrx-apps/files/ 
#  if [ $? != 0 ]; then                   
#     echo "Copy tar: $? - Unsuccessful. Location mismatch or not created tar" 
#     echo "ltrx apps built failed, so built again"
#     exit
#  fi
#fi

#if [ -f "../sources/meta-lantronix/recipes-bsp/ltrx-apps/files/ltrx-apps-sdk.tar.xz" ]; then
#  echo "Cleaning ltrx-image"
#  bitbake ltrx-image -c cleanall
#  echo "Building ltrx-image"
#  bitbake ltrx-image
#fi
bitbake ltrx-internal-image -c cleanall
bitbake ltrx-internal-image

