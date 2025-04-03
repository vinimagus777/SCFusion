# Super Fusion

Build order optimizer for StarCraft II.
Updated for patch 5.0.14 (with the caveat of the Nexus's new "mana recharge" ability, which is **not yet implemented**).

## How to Use This (If you already have Super Fusion installed on your computer)

Instructions for using the new patch.

1. Download and install from [original Repo](https://github.com/andrew-j-armstrong/SCFusion/releases).
2. Download [XML] by clicking here https://github.com/vinimagus777/SCFusion/blob/master/main/Versions/StarCraft.xml
3. Navigate to install directory **(Default is C:\Users\\\<USER>\AppData\Local\Super Fusion\Versions)**
4. Replace StarCraft.xml in the directory with the downloaded version. **WARNING!** I strongly suggest that - as good practice - you **do NOT delete/override** your current StarCraft.xml file; instead, please rename it to StarCraft.xml.old; only then, copy to your folder the StarCraft.xml file we provide here.  


## How to Use This (If you want to build this software from this repo)

1. Clone Repo
2. Download Source [wxWidgets](https://www.wxwidgets.org/downloads/)
3. Download 32-bit Binaries [wxWidgets](https://www.wxwidgets.org/downloads/) **Development Files** and **Release DLLS**. (_Click **Download Windows Binaries**_)
4. Extract binaries using 7Zip to **\SCFusion\wxWidgets**.
5. Extract source and copy **wxWidgets-<VERSION>\Include\** to **SCFusion\wxWidgets\**/
6. Download [WinSparkle](https://github.com/vslavik/winsparkle/releases/tag/v0.8.1)
7. Copy **WinSparkle.DLL** and **WinSparkle.Lib** to **\SCFusions\WinSparkle\Release\**.
8. Copy **WinSparkle\include\** to **\SCFusions\WinSparkle\**
9. VS2022 Download [Installer Projects Extensions](https://marketplace.visualstudio.com/items?itemName=VisualStudioClient.MicrosoftVisualStudio2022InstallerProjects). Older versions may also require an extension.

**Hierarchy should look like this**
  - SCFusion
    - wxWidgets
      - build
      - include
      - lib
      - wxwidgets.prop
    - WinSparkle
      - include
        - winsparkle.h
      - Release
        - WinSparkle.dll
        - WinSparkle.lib
        - WinSparkle.pdb

**Note**:
The current download for wxWidgets seems to be missing the release version of these 2 DLLS, **wxmsw32u_core_vc14x.dll**, and **wxmsw32ud_propgrid_vc14x.dll**, and will not be able to build in release mode without them. If you find them, you will need to add them as an existing item to the project, and edit their properties by changing the **Item Type** to Copy File.

