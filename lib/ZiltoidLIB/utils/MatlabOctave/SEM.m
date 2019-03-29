% https://github.com/CharleySchaefer/ZiltoidLIB
function SEM()
  close all;

  % Sample a distribution and calculate a standard error for the accuracy of the estimated probability density. 

  %================================
  % USER INPUT  
  % Settings to create a distribution
  Nsamples=400
  mu=10.0
  sigma=3.0
  distr=abs(normrnd(mu,sigma, [1, Nsamples])); % Sample a normal distribution

  % Settings for binning
  xmin=0;
  xmax=20;
  Nbins=20;

  % Settings for statistical analysis
  Nsplit=5; % In order to define a standard error, the distribution is split in Nsplit subdistributions. It should be checked that the value of Nsplit does not significantly alter the final result. 
  %================================ 


  %================================
  % MAKE HISTOGRAM OF FULL DATA SET
  x=linspace(xmin,xmax,Nbins);
  P=getProbabilityDensity(distr,  x);
  %================================


  %================================
  % GET STATISTICS
  % Split distribution in subdistributions (stored in 2D array)
  distr_random_permutation=distr(randperm(Nsamples)); 

  Nsubsamples=Nsamples/Nsplit;
  dist2D=zeros(Nsplit, Nsubsamples);
  for i=1:Nsplit
    dist2D(i, :) = distr_random_permutation((1+(i-1)*Nsubsamples):(i*Nsubsamples));
  end

  % Get normalised histograms of the 2D array
  Psub=zeros(Nsplit, Nbins);
  for i=1:Nsplit
    Psub(i, :) = getProbabilityDensity(dist2D(i, :),  x);
  end

  % Determine standard deviation (value depends on Nsplit!)
  Pstd=zeros(1, Nbins);
  for i=1:Nsplit
    Pstd(1,:) = Pstd(1,:) + (P - Psub(i,:)).^2; 
  end
  Pstd=sqrt(Pstd/Nsplit);

  % Determine standard error of mean (should not strongly depend on Nsplit, for large enough value of Nsplit)
  SEM=Pstd/sqrt(Nsplit);
  %================================


  %================================
  % PLOT RESULTS
  figure
  subplot(3,1,1)
  plot(x, exp(-0.5*( (x-mu)/(sigma) ).^2)/sqrt(2*pi*sigma^2), 'b', 'LineWidth', 2); hold on % Actual distribution
  for i=1:Nsplit
    plot(x, Psub(i, :), 'k'); hold on % Sampled subdistributions
  end
  plot(x, P, 'r', 'LineWidth', 3); hold on % Sampled distribution
  axis([xmin xmax 0 0.2])
  title('Mean distribution and subdistributions')

  subplot(3,1,2)
  plot(x, exp(-0.5*( (x-mu)/(sigma) ).^2)/sqrt(2*pi*sigma^2), 'b', 'LineWidth', 2); hold on % Actual distribution
  errorbar(x, P, Pstd, 'k'); hold on % Standard deviation
  errorbar(x, P, SEM, 'r'); hold on  % Standard error of the mean
  axis([xmin xmax 0 0.2])
  title('Standard error of the mean')

  subplot(3,1,3)
  plot(x, SEM./P, 'r', 'LineWidth', 3); hold on % Estimated error
  plot(x, abs( 1.0-P./(exp(-0.5*( (x-mu)/(sigma) ).^2)/sqrt(2*pi*sigma^2)) ), 'b', 'LineWidth', 2)   % Actual residual
  axis([xmin xmax 0 1])
  title('Reduced standard error of the mean')
  %================================
end


function P=getProbabilityDensity(distr,  x)
  Nsamples=length(distr);

  % Define bins
  if(length(x)==1)
    Nbins=x;
    xmin=min(distr);
    xmax=max(distr);
  else
    Nbins=length(x);
    xmin=min(x);
    xmax=max(x);
  end

  % Do binning
  P=zeros(1,Nbins);
  for i=1:Nsamples
    index = 1+round(  (Nbins-1)*(distr(i)-xmin)/(xmax-xmin)  );

    if(index<1)
      index=1;
    end
    if(index>Nbins)
      index=Nbins;
    end
    P(index)=P(index)+1;
  end
  
  % normalise
  P=P/Nsamples;
end
