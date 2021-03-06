#!/bin/bash

#hades
#pbeam=1.95 #T0=1.23
#pbeam=2.34 #T0=1.58

#cbm
#pbeam=2.3
#pbeam=3.3
#pbeam=5.36
#pbeam=6
#pbeam=8
#pbeam=10
pbeam=12
#pbeam=30

#ags:
#pbeam=2.78
#pbeam=4.85
#pbeam=6.87
#pbeam=8.89

#mpd:
#pbeam=9.81

#star
#pbeam=30.65
#pbeam=69.55
#pbeam=111.13

#na49/61:
#pbeam=14
#pbeam=30
#pbeam=40
#pbeam=158

#system=pau
system=auau
#system=pbpb
#system=agag

export events_per_file=1000
jobRange=1-1000
postfix=	
#partition=debug
partition=main
#partition=long

[ "$system" == "agag" ] && AP=108 && ZP=47 && AT=108 && ZT=47
[ "$system" == "auau" ] && AP=197 && ZP=79 && AT=197 && ZT=79
[ "$system" == "pbpb" ] && AP=208 && ZP=82 && AT=208 && ZT=82
[ "$system" == "pau" ] && AP=1 && ZP=1 && AT=197 && ZT=79

[ "$partition" == "debug" ] && time=0:20:00
[ "$partition" == "main" ] && time=8:00:00
[ "$partition" == "long" ] && time=1-00:00:00

export remove_logs=0 #"yes"

T0=$(echo "$pbeam" | awk '{print sqrt($pbeam*$pbeam+0.938*0.938)-0.938}')

model_source=/lustre/cbm/users/isegal/mc/macros/submit_psd_centrality/dcmqgsm_smm_stable
export root_config=/lustre/cbm/users/isegal/soft/root_v6-22-04/build/bin/thisroot.sh
export mcini_config=/lustre/cbm/users/isegal/soft/mcini/macro/config.sh
export glauber_config=/lustre/cbm/users/isegal/soft/glauber

outdir="/lustre/cbm/users/${USER}/mc/centrality/dcmqgsm_smm/${system}/pbeam${pbeam}agev${postfix}/mbias"
export outdir_root="$outdir/root/"
export outdir_dat="$outdir/dat/"
export outdir_glauber="$outdir/glauber/"
export outdir_glauber_mcini="$outdir/glauber_mcini/"
export source_dir="$outdir/src/"
export log_dir="$outdir/log/"


mkdir -p "$outdir"
mkdir -p $source_dir
mkdir -p $outdir_root
mkdir -p $outdir_dat
mkdir -p $outdir_glauber
mkdir -p $outdir_glauber_mcini
mkdir -p $log_dir

script_path=$(dirname ${0})
run_gen=${script_path}/run_gen.sh
rsync -ap --exclude=src ${model_source} $source_dir/
rsync -vp ${script_path}/input.inp.template $source_dir/dcmqgsm_smm_stable/input.inp 
rsync -vp $0 $source_dir 
rsync -vp $run_gen $source_dir 
run_gen=${source_dir}/$(basename ${run_gen})

sed -i -- "s~SRC_PATH_TEMPLATE~$source_dir/dcmqgsm_smm_stable~g" $source_dir/dcmqgsm_smm_stable/input.inp
sed -i -- "s~TO_TEMPLATE~$T0~g" $source_dir/dcmqgsm_smm_stable/input.inp
sed -i -- "s~AP_TEMPLATE~$AP~g" $source_dir/dcmqgsm_smm_stable/input.inp
sed -i -- "s~AT_TEMPLATE~$AT~g" $source_dir/dcmqgsm_smm_stable/input.inp
sed -i -- "s~ZP_TEMPLATE~$ZP~g" $source_dir/dcmqgsm_smm_stable/input.inp
sed -i -- "s~ZT_TEMPLATE~$ZT~g" $source_dir/dcmqgsm_smm_stable/input.inp
sed -i -- "s~NEVENTS_TEMPLATE~$events_per_file~g" $source_dir/dcmqgsm_smm_stable/input.inp

currentDir=`pwd`
echo "current dir:" $currentDir
echo "run_gen:" $run_gen

sbatch -J psd_centrality_$pbeam -p $partition -t $time -a $jobRange -o ${log_dir}/%a_%A.log -D $outdir -- $run_gen

echo "========================================================"
echo "Output will be written to:"
echo ""
echo "source code: $source_dir"
echo "Temporary dir (do not forget to clean up after the jobs are finished) $log_dir"
echo "root files: $outdir_root"
echo ""
echo "dat files: $outdir_dat"
echo "glauber files: $outdir_glauber"
echo "glauber mcini files: $outdir_glauber_mcini"
echo "========================================================"


