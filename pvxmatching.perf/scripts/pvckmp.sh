#!/bin/bash

BASEDIR=`dirname $0`
BASEDIR=`(cd "$BASEDIR"; pwd)`

RESULT_DIR=${BASEDIR}/../exp_result_pvc/
if [ ! -d ${RESULT_DIR} ]; then
    mkdir -p ${RESULT_DIR}
fi


N_CASE=100
MODE=pvckmp
TLEN=10000
PLEN=(1 2 3 4 5 10 15 20 25 30 35 40 45 50 100)

TALPHA=(1 2 3 4 5 10 2 4 6 8 10 20)
PALPHA=(1 2 3 4 5 10 1 2 3 4 5 10)

CMD=${BASEDIR}/../pvxperf

if [ ! -e ${CMD} ]; then
    echo "Before runnning script, you must build pvxperf."
    exit 1
fi

for ratio in {0..5}
do
    for pl in ${PLEN[@]}
    do
        for al in {0..11}
        do
            echo "ratio: ${ratio}, plen: ${pl}, talpha: ${TALPHA[al]}, palpha: ${PALPHA[al]}"
            LOG=${RESULT_DIR}/m_${MODE}_n_${N_CASE}_t_${TLEN}_p_${pl}_a_${TALPHA[al]}_b_${PALPHA[al]}_v_${ratio}
            ${CMD} -m ${MODE} -n ${N_CASE} -t ${TLEN} -p ${pl} -a ${TALPHA[al]} -b ${PALPHA[al]} -v ${ratio} > ${LOG}
        done
    done
done

