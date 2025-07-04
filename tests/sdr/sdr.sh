#! /bin/bash
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

########################################################################
#
# mozilla/security/nss/tests/sdr/sdr.sh
#
# Script to start test basic functionallity of NSS sdr
#
# needs to work on all Unix and Windows platforms
#
# special strings
# ---------------
#   FIXME ... known problems, search for this string
#   NOTE .... unexpected behavior
#
########################################################################

############################## sdr_init ################################
# local shell function to initialize this script
########################################################################
sdr_init()
{
  SCRIPTNAME=sdr.sh
  if [ -z "${CLEANUP}" ] ; then
      CLEANUP="${SCRIPTNAME}"
  fi
  
  if [ -z "${INIT_SOURCED}" -o "${INIT_SOURCED}" != "TRUE" ]; then
      cd ../common
      . ./init.sh
  fi
  SCRIPTNAME=sdr.sh

  #temporary files
  VALUE1=$HOSTDIR/tests.v1.$$
  VALUE2=$HOSTDIR/tests.v2.$$
  VALUE3=$HOSTDIR/tests.v3.$$
  VALUE4=$HOSTDIR/tests.v4.$$
  T1="Test1"
  T2="The quick brown fox jumped over the lazy dog"
  T3="1234567"
  T4="TestDES3"

  SDRDIR=${HOSTDIR}/SDR
  D_SDR="SDR.$version"
  if [ ! -d ${SDRDIR} ]; then
    mkdir -p ${SDRDIR}
  fi

  PROFILE=.
  if [ -n "${MULTIACCESS_DBM}" ]; then
     PROFILE="multiaccess:${D_SDR}"
  fi

  cd ${SDRDIR}
  html_head "SDR Tests"
}

############################## sdr_main ################################
# local shell function to test NSS SDR
########################################################################
sdr_main()
{
  # we need to make sure we are running these tests with the full
  # shipped iteration count so we can detect time regressions
  OLD_MAX_PBE_ITERATIONS=$NSS_MAX_MP_PBE_ITERATION_COUNT
  unset NSS_MAX_MP_PBE_ITERATION_COUNT
  ASCII_VALUE1=$HOSTDIR/tests.v1a.$$
  ASCII_VALUE2=$HOSTDIR/tests.v2a.$$
  ASCII_VALUE3=$HOSTDIR/tests.v3a.$$
  ASCII_VALUE4=$HOSTDIR/tests.v4a.$$
  ASCII_COMBINED=$HOSTDIR/tests.v5a.$$
  COMBINED_400=$HOSTDIR/SDR/combined.$$
  DECODED=$HOSTDIR/SDR/decoded.$$
  LOG=$HOSTDIR/SDR/log.$$

  echo "$SCRIPTNAME: Creating an SDR key/SDR Encrypt - Value 1"
  echo "sdrtest -d ${PROFILE} -o ${VALUE1} -t \"${T1}\" -f ${R_PWFILE}"
  ${BINDIR}/sdrtest -d ${PROFILE} -o ${VALUE1} -t "${T1}" -f ${R_PWFILE}
  html_msg $? 0 "Creating SDR Key/Encrypt - Value 1"

  echo "$SCRIPTNAME: SDR Encrypt - Value 2"
  echo "sdrtest -d ${PROFILE} -o ${VALUE2} -t \"${T2}\"  -f ${R_PWFILE}"
  ${BINDIR}/sdrtest -d ${PROFILE} -o ${VALUE2} -t "${T2}" -f ${R_PWFILE}
  html_msg $? 0 "Encrypt - Value 2"

  echo "$SCRIPTNAME: SDR Encrypt - Value 3"
  echo "sdrtest -d ${PROFILE} -o ${VALUE3} -t \"${T3}\"  -f ${R_PWFILE}"
  ${BINDIR}/sdrtest -d ${PROFILE} -o ${VALUE3} -t "${T3}" -f ${R_PWFILE}
  html_msg $? 0 "Encrypt - Value 3"

  echo "$SCRIPTNAME: Creating an DES3 SDR key/SDR Encrypt - Value 4"
  echo "sdrtest -d ${PROFILE} -o ${VALUE4} -t \"${T4}\"  -f ${R_PWFILE}"
  ${BINDIR}/sdrtest -d ${PROFILE} -o ${VALUE4} -t "${T4}" -f ${R_PWFILE} -m des3
  html_msg $? 0 "Creating an DES3 SDR key/Encrypt - Value 4"

  echo "$SCRIPTNAME: SDR Decrypt - Value 1"
  echo "sdrtest -d ${PROFILE} -i ${VALUE1} -t \"${T1}\"  -f ${R_PWFILE}"
  ${BINDIR}/sdrtest -d ${PROFILE} -i ${VALUE1} -t "${T1}" -f ${R_PWFILE}
  html_msg $? 0 "Decrypt - Value 1"

  echo "$SCRIPTNAME: SDR Decrypt - Value 2"
  echo "sdrtest -d ${PROFILE} -i ${VALUE2} -t \"${T2}\"  -f ${R_PWFILE}"
  ${BINDIR}/sdrtest -d ${PROFILE} -i ${VALUE2} -t "${T2}" -f ${R_PWFILE}
  html_msg $? 0 "Decrypt - Value 2"

  echo "$SCRIPTNAME: SDR Decrypt - Value 3"
  echo "sdrtest -d ${PROFILE} -i ${VALUE3} -t \"${T3}\"  -f ${R_PWFILE}"
  ${BINDIR}/sdrtest -d ${PROFILE} -i ${VALUE3} -t "${T3}" -f ${R_PWFILE}
  html_msg $? 0 "Decrypt - Value 3"

  echo "$SCRIPTNAME: DES3 SDR Decrypt - Value 4"
  echo "sdrtest -d ${PROFILE} -i ${VALUE4} -t \"${T4}\"  -f ${R_PWFILE}"
  ${BINDIR}/sdrtest -d ${PROFILE} -i ${VALUE4} -t "${T4}" -f ${R_PWFILE}
  html_msg $? 0 "DES3 Decrypt - Value 4"

  echo "$SCRIPTNAME: pwdecrypt - 300 Entries"
  # get base64 encoded encrypted versions of our tests
  sdrtest -d ${PROFILE} -o ${ASCII_VALUE1} -a -t "${T1}"  -f ${R_PWFILE}
  sdrtest -d ${PROFILE} -o ${ASCII_VALUE2} -a -t "${T2}"  -f ${R_PWFILE}
  sdrtest -d ${PROFILE} -o ${ASCII_VALUE3} -a -t "${T3}"  -f ${R_PWFILE}
  sdrtest -d ${PROFILE} -o ${ASCII_VALUE4} -a -t "${T4}"  -f ${R_PWFILE} -m des3
  # make each encoded span exactly one line
  touch ${ASCII_COMBINED}
  cat ${ASCII_VALUE1} | tr -d '\n' >> ${ASCII_COMBINED}
  echo >> ${ASCII_COMBINED}
  cat ${ASCII_VALUE2} | tr -d '\n' >> ${ASCII_COMBINED}
  echo >> ${ASCII_COMBINED}
  cat ${ASCII_VALUE3} | tr -d '\n' >> ${ASCII_COMBINED}
  echo >> ${ASCII_COMBINED}
  cat ${ASCII_VALUE4} | tr -d '\n' >> ${ASCII_COMBINED}
  echo >> ${ASCII_COMBINED}
  #concantentate the 4 entries 100 times to produce 400 entries
  touch ${COMBINED_400}
  for ((i=0;i<100;i++)); do
    cat ${ASCII_COMBINED} >> ${COMBINED_400}
  done
  echo "time pwdecrypt -i ${COMBINED_400} -o ${DECODED} -l ${LOG} -d ${PROFILE}  -f ${R_PWFILE}"
  dtime=$(time -p (pwdecrypt -i ${COMBINED_400} -o ${DECODED} -l ${LOG} -d ${PROFILE}  -f ${R_PWFILE}) 2>&1 1>/dev/null)
  echo "------------- result ----------------------"
  cat ${DECODED}
  wc -c ${DECODED}
  RARRAY=($(wc -c ${DECODED}))
  CHARCOUNT=12000
  if [ "${OS_ARCH}" = "WINNT" ]; then
    # includes include carriage returns as well as line feeds for new line
    CHARCOUNT=12400
  fi
  html_msg ${RARRAY[0]} ${CHARCOUNT} "pwdecrypt success"
  echo "------------- log ----------------------"
  cat ${LOG}
  wc -c ${LOG}
  RARRAY=($(wc -c ${LOG}))
  html_msg ${RARRAY[0]} 0 "pwdecrypt no error log"
  echo "------------- time ----------------------"
  echo $dtime
  # now parse the real time to make sure it's subsecond
  RARRAY=($dtime)
  TIMEARRAY=(${RARRAY[1]//./ })
  echo "${TIMEARRAY[0]} seconds"
  html_msg ${TIMEARRAY[0]} 0 "pwdecrypt no time regression"
  export NSS_MAX_MP_PBE_ITERATION_COUNT=$OLD_MAX_PBE_ITERATIONS
}

############################## sdr_cleanup #############################
# local shell function to finish this script (no exit since it might be
# sourced)
########################################################################
sdr_cleanup()
{
  html "</TABLE><BR>"
  cd ${QADIR}
  . common/cleanup.sh
}

sdr_init
sdr_main
sdr_cleanup
