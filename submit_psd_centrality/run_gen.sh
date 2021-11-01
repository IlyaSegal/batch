#!/bin/bash

outfilenamemask=dcmqgsm
filenum=${SLURM_ARRAY_TASK_ID}
jobDir=${log_dir}/${filenum}

mkdir -p ${jobDir}
cd ${jobDir}
echo "current dir:" $PWD

elapsed=$SECONDS
datfile=${outdir_dat}/${outfilenamemask}_${filenum}.out
rootfile=${outdir_root}/${outfilenamemask}_${filenum}.root
glauberfile=${outdir_glauber}/${outfilenamemask}_glauber_${filenum}.root
glaubermcinifile=${outdir_glauber_mcini}/${outfilenamemask}_glauber_${filenum}.mcini.root

echo datfile: ${datfile} 
echo rootfile: ${rootfile}
echo glauberfile: ${glauberfile}
echo glaubermcinifile: ${glaubermcinifile}

if [ ! -e ${glauberfile} ];then
  rm ${datfile} ${rootfile}
fi

if [ ! -e ${datfile} ];then
  seed=$(perl -e 'print int rand 99999999, "\n";')
  echo SEED: ${seed}
  echo $source_dir/dcmqgsm_smm_stable/input.inp | $source_dir/dcmqgsm_smm_stable/bin/hypcoa-b1n $seed
  $source_dir/dcmqgsm_smm_stable/bin/re-cas-smm
fi

rm outfile.*
mv CAS-SMM-evt.out ${datfile}

source $root_config
source $mcini_config
rsync -v $MCINI/macro/convertDCMQGSM_SMM.C ${jobDir}
rsync -v $MCINI/macro/convertMCGlauber.C ${jobDir}
rsync -v $glauber_config/runglauber_v3.2.C ${jobDir}
rsync -v $glauber_config/readNucleonsPositions.h ${jobDir}
rsync -v $glauber_config/rootlogon.C ${jobDir}
sed -i -- "s~const bool ifDCMSMM = false;~const bool ifDCMSMM = true;~g" ${jobDir}/runglauber_v3.2.C   
sed -i -- "s~char\* inFileName = \"\";~char\* inFileName = \"${rootfile}\";~g" ${jobDir}/runglauber_v3.2.C   
sed -i -- "s~const int nEvents = 0;~const int nEvents = ${events_per_file};~g" ${jobDir}/runglauber_v3.2.C
pwd

if [ ! -e ${rootfile} ];then
  root -b -l <<EOF
  .L convertDCMQGSM_SMM.C
  convertDCMQGSM_SMM("${datfile}", "$rootfile", $events_per_file)
EOF
fi

if [ ! -e ${glauberfile} ];then
  root -b -l <<EOF
  runAndSaveNtuple($events_per_file, "Au", "Au", 29.3, -1, 0.400, -1, -1, "$glauberfile")
EOF
fi

if [ ! -e ${glaubermcinifile} ];then
  root -b -l <<EOF
  .L convertMCGlauber.C
  convertMCGlauber("$glauberfile", "nt_Au_Au", "$glaubermcinifile")
EOF
fi

[ $remove_logs == 1 ] && rm -rf $log_dir/$filenum 

elapsed=$(expr $SECONDS - $elapsed)
echo "Done!"
echo Elapsed time: $(expr $elapsed / 60) minutes

