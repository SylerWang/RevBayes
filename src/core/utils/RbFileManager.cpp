/**
 * @file
 * This file contains the implementation of RbFileManager, which 
 * handles the opening, closing, testing presence of, etc. files.
 *
 * @brief Declaration of RbFileManager
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since version 1.0 2009-09-02
 *
 * $Id$
 */

#include "RbException.h"
#include "RbFileManager.h"
#include <iostream>
#include <string>
#include <sys/stat.h>

#ifdef WIN32
#	include <windows.h>
#else
#	include <dirent.h>
#endif




/** Default constructor, creating a file manager object with the file 
    path equal to the current (default) directory and an empty file name */
RbFileManager::RbFileManager(void) {

	setFileName("");
	setFilePath("");
	setCurDirectory( findCurrentDirectory() );
	setFilePath( getCurDirectory() );
}


/** Constructor taking as an argument a string containing a file path and (maybe) a file name */
RbFileManager::RbFileManager(std::string s) {

	setFileName("");
	setFilePath("");
	parsePathFileNames(s);
	setCurDirectory( findCurrentDirectory() );
	if ( getFilePath() == "" )
		setFilePath( getCurDirectory() );
}


/** Divides a string into the path and file name components */
bool RbFileManager::parsePathFileNames(std::string s) {

	if ( s.length() == 0 )
		{
		/* the string that is supposed to hold the
		   path/file information is empty. */
		return false;
		}
		
	/* Find the location of the last "/". This is where
	   we will divide the path/file string into two. */
	int location = (int)s.find_last_of( "/" );
	
	if ( location == -1 )
		{
		/* There is no path in this string. We 
		   must have only the file name, and the
		   file should be in our current directory. */
		fileName = s;
		filePath = "";
		}
	else if ( location == (int)s.length() - 1 )
		{
		/* It looks like the last character is "/", which
		   means that no file name has been provided. */
		s.erase( location );
		fileName = "";
		filePath = s;
		return false;
		}
	else
		{
		/* We can divide the path into the path and the file. */
		fileName = s.substr( location+1, s.length()-location-1 );
		s.erase( location );
		filePath = s;
		}

	return true;
}


/** Finds the current (default) directory of the process */
#define	MAX_DIR_PATH	2048
std::string RbFileManager::findCurrentDirectory(void) {

#	ifdef WIN32

    char buf[MAX_DIR_PATH];
    GetCurrentDirectory(MAX_DIR_PATH, buf);
    return string(buf);
	
#	else

	char cwd[MAX_DIR_PATH+1];
	if ( !getcwd(cwd, MAX_DIR_PATH+1) )
		return "";
	std::string curdir = cwd;
	
	if ( curdir.at( curdir.length()-1 ) == '/' )
		curdir.erase( curdir.length()-1 );
	
	return curdir;
    
#	endif
}


/** Tests whether the directory (passed in as an argument) is present */
bool RbFileManager::isDirectoryPresent(const std::string mp) {

#	ifdef WIN32

    WIN32_FIND_DATA data;
    HANDLE handle = FindFirstFile(mp.c_str(), &data);
    bool bFoundFile = handle != INVALID_HANDLE_VALUE;
    FindClose(handle);
    return bFoundFile;
    
#	else

	/* attempt to open the directory */
	DIR* dir = opendir( mp.c_str() );
	if ( !dir )
		return false;
		
	/* close the directory */
	if ( closedir(dir) == -1 )
        return false;
		
	return true;
    
#	endif
}


/** Checks whether the file, passed in as its path and file name components, is present */
bool RbFileManager::isFilePresent(const std::string mp, const std::string mf) {

#	ifdef WIN32

    WIN32_FIND_DATA data;
    string fullPath; 
    if (mp.length() > 1 && (mp[mp.length()-2] != '\\' || mp[mp.length()-2] != '/'))
        {
        fullPath = mp + "\\" + mf;
        }
    else
        {
        fullPath = mp + mf;
        }
    HANDLE handle = FindFirstFile(fullPath.c_str(), &data);
    bool bFoundFile = handle != INVALID_HANDLE_VALUE;
    FindClose(handle);
    return bFoundFile;
    
#	else

	/* open the directory */
	DIR* dir = opendir( mp.c_str() );
	if ( !dir )
		return false;

	/* read the directory's contents */
	struct dirent* dirEntry;
	bool foundFile = false;
	while ( (dirEntry = readdir(dir)) != NULL ) 
		{
		std::string temp = dirEntry->d_name;
		if ( temp == mf )
			foundFile = true;
		}

	/* close the directory */
	if ( closedir(dir) == -1 )
		return false;

	return foundFile;
    
#	endif
}


/** Recursively lists the contents of the directory for the member variable filePath */
bool RbFileManager::listDirectoryContents(void) {

#	ifdef WIN32

    // NOTE JH: Not certain how to list directory contents for windows
    return true;
    
#	else
    
    return listDirectoryContents(filePath);
    
#	endif
}


/** Recursively lists the contents of the directory passed in as an argument to the function */
bool RbFileManager::listDirectoryContents(const std::string& dirpath) {

    DIR* dir = opendir( dirpath.c_str() );
    if (dir)
        {
        struct dirent* entry;
        while ((entry = readdir( dir )))
            {
            struct stat entryinfo;
            std::string entryname = entry->d_name;
            std::string entrypath = dirpath + "/" + entryname;
            if (!stat( entrypath.c_str(), &entryinfo ))
                {
                if (S_ISDIR( entryinfo.st_mode ))
                    {
                    if      (entryname == "..")
                        ;
                    else if (entryname == "." ) 
                        std::cout << dirpath + "/" << std::endl;
                        //result.push_back( dirpath + "/" );
                    else
                        listDirectoryContents( entrypath );
                    }
                else
                    {
                    std::cout << entrypath << std::endl;
                    //result.push_back( entrypath );
                    }
                }
            }
        closedir( dir );
        }
    return true;
}


/** Recursively fills in a vector with the names of the files in the directory filePath */
bool RbFileManager::setStringWithNamesOfFilesInDirectory(std::vector<std::string>& sv) {

#	ifdef WIN32

    // NOTE JH: Not certain what to do here
    return true;
    
#	else

    return setStringWithNamesOfFilesInDirectory(filePath, sv);
    
#	endif
}


/** Recursively fills in a vector with the names of the files in the directory passed in as an argument to the function */
bool RbFileManager::setStringWithNamesOfFilesInDirectory(const std::string& dirpath, std::vector<std::string>& sv) {

    DIR* dir = opendir( dirpath.c_str() );
    if (dir)
        {
        struct dirent* entry;
        while ((entry = readdir( dir )))
            {
            struct stat entryinfo;
            std::string entryname = entry->d_name;
            std::string entrypath = dirpath + "/" + entryname;
            if (!stat( entrypath.c_str(), &entryinfo ))
                {
                if (S_ISDIR( entryinfo.st_mode ))
                    {
                    if (entryname == "..")
                        ;
                    else if (entryname == "." ) 
                        ;
                    else
                        setStringWithNamesOfFilesInDirectory( entrypath, sv );
                    }
                else
                    {
                    sv.push_back( entrypath );
                    }
                }
            }
        closedir( dir );
        }
    return true;
}


/** Opens a file for input */
bool RbFileManager::openFile(std::ifstream& strm) {
	
	/* concactenate path and file name */
	std::string filePathName = filePath + "/" + fileName;

	/* here we assume that the presence of the path/file has
	   been checked elsewhere */
	strm.open( filePathName.c_str(), std::ios::in );
	if ( !strm )
		return false;
	return true;
}


/** Opens a file for output */
bool RbFileManager::openFile(std::ofstream& strm) {
	
	/* concactenate path and file name */
	std::string filePathName = filePath + "/" + fileName;

	/* here we assume that the presence of the path/file has
	   been checked elsewhere */
	strm.open( filePathName.c_str(), std::ios::out );
	if ( !strm )
		return false;
	return true;
}


/** Closes an input file */
void RbFileManager::closeFile(std::ifstream& strm) {

	strm.close();
}


/** Closes an output file */
void RbFileManager::closeFile(std::ofstream& strm) {

	strm.close();
}


/** Checks whether the file name is empty */
bool RbFileManager::isFileNamePresent(void) {

    if ( fileName == "" )
        return false;
    return true;
}


/** Tests whether the directory specified in the object is present */
bool RbFileManager::testDirectory(void) {

	return isDirectoryPresent(filePath);
}


/** Tests whether the file specified in the object is present */
bool RbFileManager::testFile(void) {

	return isFilePresent(filePath, fileName);
}




