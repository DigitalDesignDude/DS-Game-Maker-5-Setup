# DS Game Maker 5 Setup for Windows 11/10

![Thumbnail_SetupDsgmOnWindows](https://user-images.githubusercontent.com/106634287/172290113-be6cfb1b-8691-4e6e-a118-03230c9ca70b.png)


## About DS Game Maker

DS Game Maker (DSGM) is early 2000's software for developing Nintendo DS homebrew games with basic drag-and-drop visual coding, the C programming language or with its own language called DBAS. It was developed By James Garner and has been tested to still run on Windows 7, 10 and now 11 computers using this setup tutorial by DigitalDesignDude.
<br>
<br>
<br>



## 1st Step - DevkitPro Setup
  1. Download the "devkitPro" folder and place it on the root of C drive of your computer. Thus the devkitPro folder's location will be "C:\devkitPro"
	
If you don't have a C drive, you might be able to put the "devkitPro" folder on the root of another drive, but you'll have to make sure to use the appropriate drive letter instead of "C" when creating the system environment variables later on in this tutorial. 

<br>
<br>



## 2nd Step - DS Game Maker Setup

  1. Download the "DS Game Maker" folder and place it anywhere you want on your PC so long as it's on the same drive as the devkitPro folder. For example, assuming you placed the devkitPro folder on your C Drive, you should be able to place the "DS Game Maker" folder in your "Documents" folder without any issues. 
	
  2. Within the "DS Game Maker" folder make the "DS Game Maker.exe" file always  run in administrator mode by right clicking it then choosing "properties" from the right click menu options and then checking the "Run this program as an administrator" option from under the Compatibility tab in the file properties window.
<br>
<br>



## 3rd Step - Add the Proper PC Environment Variables DSGM Needs

PC environment variables provide extra information that apps may need to in order to find other files or data on your computer and it would normally be added for you when you install a program using a program installer file.

Since the DSGM 5.20 installer that was released years ago installs a buggy version of DSGM 5.20 and doesn't add the proper environment variables we'll be adding them ourselves to the Windows 10/11 Environment Variables menu.
<br>
  1. Using the Windows 10 or 11 search bar search for "edit the system environment variables"  and click on that option.
  
  2.  When the new window opens click the "Environment Variables..." button to be brought to where you can edit or create the needed system environment variables below.

<br>
NOTE: the environment variables and values are case sensitive so they must be entered or pasted in exactly as shown below. If you placed the devkitPro folder on a drive different than the C drive, then be sure to use the appropriate drive letter instead of "c" for the variables' values.
<br>
<br>
<br>
	
	System Variable: DEVKITPRO
	Value: /c/devkitPro
	
	System Variable: DEVKITARM
	Value: /c/devkitPro/devkitARM
	
	System Variable: Path
	Values Added:
	c:\devkitPro\msys\bin
	C:\devkitPro\devkitARM\bin

	
	NOTE: Just add the above value to the "Path" Variable because it may contain multiple values already.
	
<br>
<br>

## 4th Step - Ensure You Have Microsoft .NET Framework 3.5 Installed

Certain Windows apps require Microsoft .Net Framework 3.5 in order to function so it may already be installed on your computer. But we should verify that and install it if it's not already, otherwise DS Game Maker will not work properly.

1. Using the Windows 10 or 11 search bar search for the "Turn Windows features on and off" option and click on it.

2. In the "Turn Windows features on and off" menu, ensure that the check box besides ".NET Framework 3.5 (includes .Net 2.0 and 3.0)" is filled. If not, click the check box to proceed to download and install .NET Framework 3.5.
<br>
<br>

## LAST Step - Test Making a Nintendo DS Game File with DSGM
1. Run the DSGameMaker.exe and click "yes" if prompted by the Windows administrator permission request popup.

2. Test compiling a blank project by pressing the "Test Game" button from the top ribbon of DSGM's interface (it looks like a play button).

Congrats! If you have no errors you're ready to start building Nintendo DS games with DS Game Maker!
<br>
<br>
<br>

# DSGM Knowlegde Resources!
![banner_DsgmKnowlegdeResources](https://user-images.githubusercontent.com/106634287/173729565-1b842d40-cd44-4376-91d4-6eb6a5515957.png)
<br>
<br>

Check out the [DS Game Maker Resource Site](https://digitaldesigndude.github.io/DSGM-Resource-Site/index.html) for documentation, tutorials, community links and more!

