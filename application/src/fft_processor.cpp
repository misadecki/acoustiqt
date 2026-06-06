#include "fft_processor.hh"
#include <cmath>
#include "audio_config.hh"
#include <QDebug>

FFTProcessor::FFTProcessor(QObject *parent) : QObject(parent) {
  fft_in = static_cast<double*>(fftw_malloc(sizeof(double) * AudioConfig::N));
  fft_out = static_cast<fftw_complex*>(fftw_malloc(sizeof(fftw_complex) * AudioConfig::N));
  fft_plan = fftw_plan_dft_r2c_1d(AudioConfig::N, fft_in, fft_out, FFTW_ESTIMATE);
}

QList<double> FFTProcessor::calculateMag() {
  QList<double> spectrum;
  spectrum.reserve(AudioConfig::N / 2);
  for (uint16_t i = 0; i < AudioConfig::N / 2; ++i) {
    double real = fft_out[i][0];
    double imag = fft_out[i][1];
    double mag = sqrt(real * real + imag * imag);
    spectrum.append(mag);
  }
  // qDebug() << "Magnitude: " << spectrum[0];
  return spectrum;
}

double FFTProcessor::calculateMean(const QList<int32_t> &raw_samples) {
  if (raw_samples.isEmpty()) return 0.0;
  double sum = 0.0;
  for (int32_t sample : raw_samples)
    sum += static_cast<double>(sample);

  return sum / raw_samples.size();
}

void FFTProcessor::handleRawAudio(const QList<int32_t> &raw_samples) {
  if (raw_samples.size() < AudioConfig::N) return;

  double mean = calculateMean(raw_samples);

  for (int32_t i = 0; i < AudioConfig::N; ++i) {
    double centered = (static_cast<double>(raw_samples[i]) - mean) /
      AudioConfig::MAX_INT32_T;
    double window = 0.5 * (1.0 - cos(2.0 * M_PI * i) / AudioConfig::N);
    fft_in[i] = centered * window * volume_gain;
  }

  fftw_execute(fft_plan);
  AudioStats stats;
  QList<double> magnitudes = calculateMag();
  stats.dominant_freq = calculateDominantFreq(magnitudes);
  stats.rms = calculateRMS(raw_samples);
  stats.zcr = calculateZCR(raw_samples);
  stats.peak = calculatePeak(raw_samples);
  stats.dbfs = getDecibels(raw_samples);
  emit spectrumReady(magnitudes);
  emit statsReady(stats);
}

double FFTProcessor::calculateRMS(const QList<int32_t> &raw_samples) {
  if (raw_samples.isEmpty()) return 0.0;

  double mean = calculateMean(raw_samples);

  double sum = 0.0;
  for (int32_t sample : raw_samples) {
    double centered = static_cast<double>(sample) - mean;
    double normalized = (centered / AudioConfig::MAX_INT32_T) * volume_gain;
    sum += normalized * normalized;
  }
  
  return std::sqrt(sum / raw_samples.size());
}

double FFTProcessor::getDecibels(const QList<int32_t> &raw_samples) { 
  double rms = calculateRMS(raw_samples);
  if (rms < 0.00001) return -AudioConfig::NOISE_THRESHOLD;
  return 20 * std::log10(rms);
}

double FFTProcessor::calculateDominantFreq(const QList<double> &magnitudes) {
  if (magnitudes.size() < 4) return 0.0;
  QList<double>::const_iterator it_start = magnitudes.begin() + 3;
  QList<double>::const_iterator it_end = magnitudes.begin() + (magnitudes.size() / 2);

  QList<double>::const_iterator it = std::max_element(it_start, it_end);
  uint16_t max_idx = std::distance(magnitudes.begin(), it);

  return max_idx * (static_cast<double>(AudioConfig::SAMPLE_RATE) /
                    AudioConfig::N);
}

uint16_t FFTProcessor::calculateZCR(const QList<int32_t> &raw_samples) {
  if (raw_samples.isEmpty()) return 0.0;

  uint16_t crossings = 0;
  for (int32_t i = 1; i < AudioConfig::N; ++i) {
    if (raw_samples[i - 1] > 0 && raw_samples[i] < 0)
      ++crossings;
    else if (raw_samples[i - 1] < 0 && raw_samples[i] > 0)
        ++crossings;
  }
  return crossings;
}

double FFTProcessor::calculatePeak(const QList<int32_t> &raw_samples) {
  if (raw_samples.isEmpty()) return 0.0;
  double current_peak = 0.0;
  double mean = calculateMean(raw_samples);

  for (int32_t sample : raw_samples) {
    double centered = static_cast<double>(sample) - mean;
    double normalized = (std::abs(centered) / AudioConfig::MAX_INT32_T) *
      volume_gain;

    if (normalized > current_peak)
      current_peak = normalized;
  }

  return current_peak;
}

FFTProcessor::~FFTProcessor() {
  fftw_destroy_plan(fft_plan);
  fftw_free(fft_in);
  fftw_free(fft_out);
}
