!exists('./tmp'){
	message("No directory ./tmp")
	system('mkdir tmp'){message('Directory ./tmp created.')}
	else{
		error('Directory ./tmp not created!!! Create directory ./tmp yourSelf.')
	}
}
!exists('./tmp/configSalary'){
	message("No directory ./tmp/configSalary")
	system('mkdir .\tmp\configSalary'){message('Directory ./tmp/configSalary created.')}
	else{
		error('Directory ./tmp/configSalary not created!!! Create directory ./tmp/configSalary yourSelf.')
	}
}

!exists('./tmp/localPro'){
	message("No directory ./tmp/localPro")
	system('mkdir .\tmp\localPro'){message('Directory ./tmp/localPro created.')}
	else{
		error('Directory ./tmp/localPro not created!!! Create directory ./tmp/localPro yourSelf.')
	}
}

system(cd src & qmake salaryLocal.pro & qmake salaryNetwork.pro & cd configSalary & qmake & cd ../..)

CONFIG		+= release
CONFIG		-= debug debug_and_release debug_and_release_target

TEMPLATE    = subdirs
SUBDIRS     = src/salaryNetwork.pro  src/configSalary
	
#SUBDIRS     + src/salaryLocal.pro