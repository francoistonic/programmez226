CString SearchDrive(const CString& strFile, const CString& strFilePath, const bool& bRecursive, const bool& bStopWhenFound) 
{
	CString strFoundFilePath;
	WIN32_FIND_DATA file;

	CString strPathToSearch = strFilePath;
	strPathToSearch += _T("\\");

	HANDLE hFile = FindFirstFile((strPathToSearch + "*"), &file);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			CString strTheNameOfTheFile = file.cFileName;

			// It could be a directory we are looking at
			// if so look into that dir
			if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if ((strTheNameOfTheFile != ".") && 
					(strTheNameOfTheFile != "..") && 
					(bRecursive))
				{
					++ullFolders;
					strFoundFilePath = SearchDrive(strFile, strPathToSearch + strTheNameOfTheFile, 
						bRecursive, bStopWhenFound);

					if (!strFoundFilePath.IsEmpty() && bStopWhenFound)
						break;
				}
			}
			else
			{
				++ullFiles;
				strFoundFilePath = strPathToSearch + strTheNameOfTheFile; //strFile;

				ULONGLONG size = (static_cast<ULONGLONG>(file.nFileSizeHigh) <<
					sizeof(file.nFileSizeLow) * 8) | file.nFileSizeLow;

				TCHAR szPath[10240];
				memset(szPath, 0, 10240);
				_tcscpy_s(szPath, (LPCTSTR)strFoundFilePath);
				::PathRemoveFileSpec(szPath);
				_tprintf(_T("%s;%s;%lld\n"), szPath, (LPCTSTR)strTheNameOfTheFile, size);


				if (bStopWhenFound)
					break;
			}
		} while (FindNextFile(hFile, &file));

		FindClose(hFile);
	}

	return strFoundFilePath;
}
