{
  "targets": [
	  {
		  "target_name": "nlpir",
		  'include_dirs': [ "NLPIR/include" ],
		  'defines': ['OS_LINUX',],
		  'libraries': [
		  		'/home/tyt/ictclas/NLPIR/bin/libNLPIR.so',
		  ],
		  "sources": [ "nlpir.cpp" ],
	  },

  ],
}

