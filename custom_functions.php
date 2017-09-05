<?php
/**
 * @file
 * Provide custom functions for user by the module
 *
 * @author Matheus Campos da Silva, 2017
 */

/**
 * Get current semester
 *
 * @param int $month
 *  The current month
 *
 * @return $semester
 *  Current period based in month param
 */

function getSemester($month)
{
  # May not always be right due to strikes
  # and/or occupations
  $semester = null;
  if ($month <= 6) {
    $semester = 1;
  } else {
    $semester = 2;
  }
  return $semester;
}
