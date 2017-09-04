prefix=$3
read countN1 countN2 <<<$(./GalledTD $1 $2 $3)

# rcountAstar

read gt_LL1LL2 gf_LL1LL2 <<<$(./trip-dist $prefix/N1_NLL $prefix/N2_NLL)
read gt_LL1L2 gf_LL1L2 <<<$(./trip-dist $prefix/N1_NLL $prefix/N2_NL)
read gt_LL1RR2 gf_LL1RR2 <<<$(./trip-dist $prefix/N1_NLL $prefix/N2_NRR)
read gt_LL1R2 gf_LL1R2 <<<$(./trip-dist $prefix/N1_NLL $prefix/N2_NR)

read gt_L1LL2 gf_L1LL2 <<<$(./trip-dist $prefix/N1_NL $prefix/N2_NLL)
read gt_L1L2 gf_L1L2 <<<$(./trip-dist $prefix/N1_NL $prefix/N2_NL)
read gt_L1RR2 gf_L1RR2 <<<$(./trip-dist $prefix/N1_NL $prefix/N2_NRR)
read gt_L1R2 gf_L1R2 <<<$(./trip-dist $prefix/N1_NL $prefix/N2_NR)

read gt_RR1LL2 gf_RR1LL2 <<<$(./trip-dist $prefix/N1_NRR $prefix/N2_NLL)
read gt_RR1L2 gf_RR1L2 <<<$(./trip-dist $prefix/N1_NRR $prefix/N2_NL)
read gt_RR1RR2 gf_RR1RR2 <<<$(./trip-dist $prefix/N1_NRR $prefix/N2_NRR)
read gt_RR1R2 gf_RR1R2 <<<$(./trip-dist $prefix/N1_NRR $prefix/N2_NR)

read gt_R1LL2 gf_R1LL2 <<<$(./trip-dist $prefix/N1_NR $prefix/N2_NLL)
read gt_R1L2 gf_R1L2 <<<$(./trip-dist $prefix/N1_NR $prefix/N2_NL)
read gt_R1RR2 gf_R1RR2 <<<$(./trip-dist $prefix/N1_NR $prefix/N2_NRR)
read gt_R1R2 gf_R1R2 <<<$(./trip-dist $prefix/N1_NR $prefix/N2_NR)

rcountAstar=$(((gt_LL1LL2-gt_LL1L2+gt_LL1RR2-gt_LL1R2) - (gt_L1LL2-gt_L1L2+gt_L1RR2-gt_L1R2) + (gt_RR1LL2-gt_RR1L2+gt_RR1RR2-gt_RR1R2) - (gt_R1LL2-gt_R1L2+gt_R1RR2-gt_R1R2)))

# rcountA
read gt_W1LL2 gf_W1LL2 <<<$(./trip-dist $prefix/N1_Nsw $prefix/N2_NLL)
read gt_W1L2 gf_W1L2 <<<$(./trip-dist $prefix/N1_Nsw $prefix/N2_NL)
read gt_W1RR2 gf_W1RR2 <<<$(./trip-dist $prefix/N1_Nsw $prefix/N2_NRR)
read gt_W1R2 gf_W1R2 <<<$(./trip-dist $prefix/N1_Nsw $prefix/N2_NR)

read gt_E1LL2 gf_E1LL2 <<<$(./trip-dist $prefix/N1_Nse $prefix/N2_NLL)
read gt_E1L2 gf_E1L2 <<<$(./trip-dist $prefix/N1_Nse $prefix/N2_NL)
read gt_E1RR2 gf_E1RR2 <<<$(./trip-dist $prefix/N1_Nse $prefix/N2_NRR)
read gt_E1R2 gf_E1R2 <<<$(./trip-dist $prefix/N1_Nse $prefix/N2_NR)

read gt_D1LL2 gf_D1LL2 <<<$(./trip-dist $prefix/N1_Ns $prefix/N2_NLL)
read gt_D1L2 gf_D1L2 <<<$(./trip-dist $prefix/N1_Ns $prefix/N2_NL)
read gt_D1RR2 gf_D1RR2 <<<$(./trip-dist $prefix/N1_Ns $prefix/N2_NRR)
read gt_D1R2 gf_D1R2 <<<$(./trip-dist $prefix/N1_Ns $prefix/N2_NR)

read gt_W2LL1 gf_W2LL1 <<<$(./trip-dist $prefix/N2_Nsw $prefix/N1_NLL)
read gt_W2L1 gf_W2L1 <<<$(./trip-dist $prefix/N2_Nsw $prefix/N1_NL)
read gt_W2RR1 gf_W2RR1 <<<$(./trip-dist $prefix/N2_Nsw $prefix/N1_NRR)
read gt_W2R1 gf_W2R1 <<<$(./trip-dist $prefix/N2_Nsw $prefix/N1_NR)
gtA_W2=$(((gt_W2LL1-gt_W2L1) + (gt_W2RR1-gt_W2R1)))

read gt_E2LL1 gf_E2LL1 <<<$(./trip-dist $prefix/N2_Nse $prefix/N1_NLL)
read gt_E2L1 gf_E2L1 <<<$(./trip-dist $prefix/N2_Nse $prefix/N1_NL)
read gt_E2RR1 gf_E2RR1 <<<$(./trip-dist $prefix/N2_Nse $prefix/N1_NRR)
read gt_E2R1 gf_E2R1 <<<$(./trip-dist $prefix/N2_Nse $prefix/N1_NR)
gtA_E2=$(((gt_E2LL1-gt_E2L1) + (gt_E2RR1-gt_E2R1)))

read gt_D2LL1 gf_D2LL1 <<<$(./trip-dist $prefix/N2_Ns $prefix/N1_NLL)
read gt_D2L1 gf_D2L1 <<<$(./trip-dist $prefix/N2_Ns $prefix/N1_NL)
read gt_D2RR1 gf_D2RR1 <<<$(./trip-dist $prefix/N2_Ns $prefix/N1_NRR)
read gt_D2R1 gf_D2R1 <<<$(./trip-dist $prefix/N2_Ns $prefix/N1_NR)
gtA_D2=$(((gt_D2LL1-gt_D2L1) + (gt_D2RR1-gt_D2R1)))

rcountA=$(((gt_W1LL2-gt_W1L2) + (gt_W1RR2-gt_W1R2) + (gt_E1LL2-gt_E1L2) + (gt_E1RR2-gt_E1R2) - (gt_D1LL2-gt_D1L2) - (gt_D1RR2-gt_D1R2) + rcountAstar))

# rcount
read gt_W1W2 gf_W1W2 <<<$(./trip-dist $prefix/N1_Nsw $prefix/N2_Nsw)
read gt_E1W2 gf_E1W2 <<<$(./trip-dist $prefix/N1_Nse $prefix/N2_Nsw)
read gt_D1W2 gf_D1W2 <<<$(./trip-dist $prefix/N1_Ns $prefix/N2_Nsw)

read gt_W1E2 gf_W1E2 <<<$(./trip-dist $prefix/N1_Nsw $prefix/N2_Nse)
read gt_E1E2 gf_E1E2 <<<$(./trip-dist $prefix/N1_Nse $prefix/N2_Nse)
read gt_D1E2 gf_D1E2 <<<$(./trip-dist $prefix/N1_Ns $prefix/N2_Nse)

read gt_W1D2 gf_W1D2 <<<$(./trip-dist $prefix/N1_Nsw $prefix/N2_Ns)
read gt_E1D2 gf_E1D2 <<<$(./trip-dist $prefix/N1_Nse $prefix/N2_Ns)
read gt_D1D2 gf_D1D2 <<<$(./trip-dist $prefix/N1_Ns $prefix/N2_Ns)

rcount=$((gt_W1W2 + gt_E1W2 - gt_D1W2 + gtA_W2 + gt_W1E2 + gt_E1E2 - gt_D1E2 + gtA_E2 - gt_W1D2 - gt_E1D2 + gt_D1D2 - gtA_D2 + rcountA))

# fcountBCstar
read gt_B1B2 gf_B1B2 <<<$(./trip-dist $prefix/N1_NB $prefix/N2_NB)
read gt_B1C2 gf_B1C2 <<<$(./trip-dist $prefix/N1_NB $prefix/N2_NC)
read gt_C1B2 gf_C1B2 <<<$(./trip-dist $prefix/N1_NC $prefix/N2_NB)
read gt_C1C2 gf_C1C2 <<<$(./trip-dist $prefix/N1_NC $prefix/N2_NC)
fcountBCstar=$((gf_B1B2 + gf_B1C2 + gf_C1B2 + gf_C1C2))

# fcountBC
read gt_W2B1 gf_W2B1 <<<$(./trip-dist $prefix/N2_Nsw $prefix/N1_NB)
read gt_W2C1 gf_W2C1 <<<$(./trip-dist $prefix/N2_Nsw $prefix/N1_NC)
gf_W2BC1=$((gf_W2B1+gf_W2C1))
read gt_W1B2 gf_W1B2 <<<$(./trip-dist $prefix/N1_Nsw $prefix/N2_NB)
read gt_W1C2 gf_W1C2 <<<$(./trip-dist $prefix/N1_Nsw $prefix/N2_NC)
gf_W1BC2=$((gf_W1B2+gf_W1C2))

read gt_E2B1 gf_E2B1 <<<$(./trip-dist $prefix/N2_Nse $prefix/N1_NB)
read gt_E2C1 gf_E2C1 <<<$(./trip-dist $prefix/N2_Nse $prefix/N1_NC)
gf_E2BC1=$((gf_E2B1+gf_E2C1))
read gt_E1B2 gf_E1B2 <<<$(./trip-dist $prefix/N1_Nse $prefix/N2_NB)
read gt_E1C2 gf_E1C2 <<<$(./trip-dist $prefix/N1_Nse $prefix/N2_NC)
gf_E1BC2=$((gf_E1B2+gf_E1C2))

read gt_D2B1 gf_D2B1 <<<$(./trip-dist $prefix/N2_Ns $prefix/N1_NB)
read gt_D2C1 gf_D2C1 <<<$(./trip-dist $prefix/N2_Ns $prefix/N1_NC)
gf_D2BC1=$((gf_D2B1+gf_D2C1))
read gt_D1B2 gf_D1B2 <<<$(./trip-dist $prefix/N1_Ns $prefix/N2_NB)
read gt_D1C2 gf_D1C2 <<<$(./trip-dist $prefix/N1_Ns $prefix/N2_NC)
gf_D1BC2=$((gf_D1B2+gf_D1C2))

fcountBC=$((gf_W1BC2 + gf_E1BC2 - gf_D1BC2 + fcountBCstar))

# fcount
fcount=$((gf_W1W2 + gf_E1W2 - gf_D1W2 + gf_W2BC1 + gf_W1E2 + gf_E1E2 - gf_D1E2 + gf_E2BC1 - gf_W1D2 - gf_E1D2 + gf_D1D2 - gf_D2BC1 + fcountBC))

# count
count=$((rcount + fcount))

echo $((countN1 + countN2 - 2*count))

